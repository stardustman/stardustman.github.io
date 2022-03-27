git rebase -i HEAD~N

> 复制提交记录

git branch -f target-branch-name commit-id

> 切换到 target-branch-name 并指向 commit-id

git cherry-pick commit-id1 commit-id2

> 重新排序提交的记录. cherry-pick 名字起得真是有意思啊. 像捡樱桃似的, 把一个个想要的记录捡到当前分支的后面.

git checkout -b branch-name

> 创建一个新分支, 并切换到新建的分支

git merge target-branch-name

> 合并 target-branch-name 到当前分支. 结果是创建一个新的提交记录.

git checkout target-commit-id

> detach HEAD, let *HEAD* point to target-commit-id.

git tag tag-name target-commit-id

> 在 target-commit-id 处, 打上 tag-name 标签.

# reference

1. [learning git branch](https://learngitbranching.js.org/)
