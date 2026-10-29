# upstream

Kernel trees, shallow. Mainline and LTS: `linux-stable.git` (bare) with a worktree per release (`git -C linux-stable.git fetch --depth 1 --no-tags origin tag <tag>` then `worktree add --detach ../linux-<ver> <tag>`). Vendor forks: one folder each, fetched at the pinned commit. Boards name the folder in `KERNEL_TREE`.
