// @NAME@: Linux userspace program in Zig, cross-compiled for the board (riscv64-linux-musl, static), no libc
// calls: raw Linux syscalls from std.os.linux. Reads the kernel release from /proc/sys/kernel/osrelease and writes
// "@NAME@: running on Linux <release>" to the kernel log (/dev/kmsg), which test.cmds checks with klog.
const std = @import("std");
const linux = std.os.linux;

fn openPath(path: [*:0]const u8, flags: linux.O) !i32 {
    const rc = linux.open(path, flags, 0);
    if (linux.errno(rc) != .SUCCESS) return error.OpenFailed;
    return @intCast(rc);
}

pub fn main() !void {
    var release: [64]u8 = undefined;
    const fd = try openPath("/proc/sys/kernel/osrelease", .{ .ACCMODE = .RDONLY });
    const n = linux.read(fd, &release, release.len);
    _ = linux.close(fd);
    if (linux.errno(n) != .SUCCESS or n == 0) return error.ReadFailed;
    const rel = std.mem.trimEnd(u8, release[0..n], "\n");

    var line: [128]u8 = undefined;
    const msg = try std.fmt.bufPrint(&line, "@NAME@: running on Linux {s}\n", .{rel});
    const kmsg = try openPath("/dev/kmsg", .{ .ACCMODE = .WRONLY });
    _ = linux.write(kmsg, msg.ptr, msg.len);
    _ = linux.close(kmsg);
}
