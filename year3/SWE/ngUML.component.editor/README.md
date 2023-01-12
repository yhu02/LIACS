<h1 align="center">
  ngUML.editor
</h1>
<p align="center">React editor for interfacing with the <a href="https://github.com/ludev-ng-nl/ngUML.backend">ngUML.backend</a>.</p>

## 📦 Requirements

Make sure that you have **nodejs** installed at least the most recent LTS version. 

We recommend installing through [NVM](https://github.com/nvm-sh/nvm#install--update-script), a **nodejs** version manager. After installation, you can install the lts-version and use it
for this project:

```bash
# Make sure that you have NVM
nvm --version

# Install latest LTS-version
nvm install --lts

# Use the LTS-version in this project
nvm use --lts
```

## ⚡️ Quick start

> ☝️ This project presumes you are running a POSIX-based system.

```bash
# Ensure that you have the required tools 
node --version  # > v16.17.1
yarn --version  # > 1.22.19 (if yarn is not found, install: `npm install -g yarn`)

# Clone the project
git clone git@github.com:ludev-ng-nl/ngUML.editor.git
cd ./ngUML.editor

# Install dependencies
yarn install

# Run the development server
yarn dev
```

The development server should now be available at
`http://127.0.0.1:5173/`
