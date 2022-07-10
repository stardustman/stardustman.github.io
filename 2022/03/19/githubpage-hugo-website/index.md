
# 步骤

## 安装软件

1. [git](https://git-scm.com/)

```bash
  # 配置好 git
  git config --global user.name "your username"
  git config --global user.email "your email"
```

2. [hugo](https://gohugo.io/getting-started/installing/)

## hugo 创建本地 website

> 参考：<https://gohugo.io/getting-started/quick-start/>

1. hugo new site blog # blog 根据需要修改
2. cd blog
3. git init
4. git submodule add <https://github.com/theNewDynamic/gohugo-theme-ananke.git> themes/ananke # theme 根据需要修改
5. 把 content/posts 目录修改为 content/post
6. hugo new post/my-first-post.md
7. hugo server -D
8. 根据选择的主题，修改主题相关的配置

## github 上创建两个公开仓库

1. `username.github.io`, 部署静态博客的仓库，username 修改为对应的 github 账号 ID。
2. `blog`，这个保存本地的 `blog` 的仓库。
3. 在 `blog` 中执行以下命令:
   ```bash
   git remote add origin git@github.com:username/blog.git # username 修改为自己账号的 github ID
   git branch -M main
   git push -u origin main
   ```

## 配置 github actions

> 参考：<https://github.com/peaceiris/actions-gh-pages#%EF%B8%8F-first-deployment-with-github_token>
> 使用 actions-hugo 和 actions-gh-pages 自动化部署 `blog`
> 生成的密钥对本地保存就可以。

1. 生成一对 public key(gh-pages.pub) 和 private key(gh-pages)

```bash
 ssh-keygen -t rsa -b 4096 -C "$(git config user.email)" -f gh-pages -N ""
 #  gh-pages.pub  公钥
 #  gh-pages      私钥
 ```

2. 添加公钥 `gh-pages.pub` 添加到 username.github.io 这个仓库的 deploy key 中，勾选 `Allow write access`

> <https://github.com/username/username.github.io/settings/keys/new>

3. 添加私钥 `gh-pages` 到 `blog` 中的 secret

> <https://github.com/username/blog/settings/secrets/actions/new>

1. `Name` ACTIONS_DEPLOY_KEY
2. `Value` gh-pages 私钥文件内容

## 本地 `blog` 添加 deploy.yml

1. mkdir -p .github/workfows/
2. touch .github/workflows/deploy.yml
3. 粘贴一下内容

```yaml
name: GitHub Pages

on:
  push:
    branches:
      - main  # Set a branch to deploy
  pull_request:

jobs:
  deploy:
    runs-on: ubuntu-20.04
    concurrency:
      group: ${{ github.workflow }}-${{ github.ref }}
    steps:
      - uses: actions/checkout@v3
        with:
          submodules: true  # Fetch Hugo themes (true OR recursive)
          fetch-depth: 0    # Fetch all history for .GitInfo and .Lastmod

      - name: Setup Hugo
        uses: peaceiris/actions-hugo@v2
        with:
          hugo-version: '0.91.2'
          # extended: true

      - name: Build
        run: hugo --minify

      # https://github.com/peaceiris/actions-gh-pages#%EF%B8%8F-deploy-to-external-repository-external_repository
      - name: Deploy
        uses: peaceiris/actions-gh-pages@v3
        with:
          deploy_key: ${{ secrets.ACTIONS_DEPLOY_KEY }}
          external_repository: username/username.github.io # username 替换为 github ID
          publish_branch: master  # default: gh-pages, 注意修改为部署的分支
          publish_dir: ./public
```

## 提交 本地 blog 到 github blog 仓库

```bash
git add .
git commit -m "commit info"
git push
```

# Reference

1. [github pages](https://zhuanlan.zhihu.com/p/38480155)
2. [hugo quickstart](https://gohugo.io/getting-started/quick-start/)
3. [actions-hugo](https://github.com/peaceiris/actions-hugo)
4. [actions-gh-pages](https://github.com/peaceiris/actions-gh-pages)
5. [deploy-to-external-repository-external_repository](https://github.com/peaceiris/actions-gh-pages#%EF%B8%8F-deploy-to-external-repository-external_repository)
