# Devkit Bootstrapper
For quickly unpacking and setting up my development toolkit on any Windows computer

## Supported Platforms
Only Windows

## Usage

**Setting up the Bootstrapper**

1. Get a flash drive
2. Move a built binary of the bootstrapper into the drive
3. Move an installation of Neovim into the drive under the directory `neovim/inst`
4. Move your Neovim configuration into the drive under the directory `neovim/conf`
5. Move an installation of Alacritty into the drive under the directory `alacritty/inst`
6. Move your Alacritty configuration into the drive under the directory `alacritty/conf`

**Configuration file macros**

* `BOOTSTRAP_BINARY_DIR` - The directory the program binaries are found
* `BOOTSTRAP_CONFIG_DIR` - The directory the program configuration files are found

**Using the Bootstrapper**

1. Copy out all files from the flash drive
2. Run the bootstrapper binary with the `install` argument
3. You can now launch Alacritty through the binaries in `alacritty/inst`
4. After finishing, run the bootstrapper binary again with the `uninstall` argument
5. You can now delete the bootstrapper binary
