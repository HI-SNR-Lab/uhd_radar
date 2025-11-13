# Step-by-step Installation Guide
The following guide is for **Windows**, however the steps are the same for Linux and MacOS with some minor tweaks (no need to install WSL, filepaths may differ, etc).
### Installing Software
1. Install **Visual Studio Code (VSCode)** or another code editor of your choice.

   If you are using VSCode, see the additional steps for setting it up [here](vscode.md).
2. Install **Windows Subsystem for Linux (WSL)**. ORCA is developed to be used on a Linux system and requires a Linux terminal such as WSL to run.

    From the Start menu, open **Powershell** and run ```wsl --install```.
3. Download **Conda for Linux**. In this guide, we use **Miniconda**. More details about Conda can be found [here](conda.md). You do not need to open or run the ```.sh``` file at this time.
4. Within VSCode, open a new Ubuntu (WSL) terminal. You may be prompted to create a username and password for the Linux installation. This password will be used any time you run WSL terminal commands with administrator permissions (```sudo```).
5. In the WSL terminal, run `bash <path-to-Downloads>/Miniconda3-latest-Linux_x86_64.sh`. The path to your downloads folder is typically `/mnt/c/Users/<your-username>/Downloads`. Accept the default options when prompted, and select **yes** when asked about `auto_activate_base`, though we will change this setting later.
6. After Miniconda has been installed, close and reopen the WSL terminal, then run `conda list` to confirm everything has been installed correctly. You should see a long list of all the installed dependencies printed to the terminal. If you get an error saying _conda: command not found_, you may be in the wrong terminal or did not install it correctly.
7. We are now going to disable conda from activating every time you open a new terminal. To do this, run `conda config --set auto_activate_base false`.
### Connecting to GitHub
**If you do not intend to modify the code in any way, you may skip this section.**
1. Create an account on [GitHub](https://github.com/signup) and sign in.
2. Go to the [ORCA Repository](https://github.com/radioglaciology/uhd_radar) and click **Fork** to create your own copy of the code. Name this new repository whatever you want.
3. To securely connect your computer to GitHub we will create an SSH key. In the WSL terminal, run `ssh-keygen -t ed25519 -C "your@email.com"` and accept the default location.
4. Create a password that you will input each time you push or pull code from GitHub. When you type the password, no text will appear in the terminal but the password is being logged.
5. We now need to copy the public key you just created. When the key was generated, a message should have been printed that says something like _Your public key has been saved in /home/<your-username>/.ssh/id_ed25519.pub_. To view the contents of this public key file, run `cat /home/<your-username>/.ssh/id_ed25519.pub` and copy the full key that is printed to the terminal.
6. In GitHub. open **Settings > SSH and GPG Keys**, and click **New SSH Key**, then paste the key into the appropriate box.
### Cloning the Repository
1. We now need to install Git in WSL. Run `sudo apt update && sudo apt install git`.
2. To be able to access the code from GitHub on your computer, we have to clone the repository. Go to the fork you just created (or the ORCA repository if you are not creating a fork), and click the green **Code** button. Within the dropdown, go to the **SSH** tab and copy the link. It should end in .git.
3. Navigate (cd) to whichever folder you would like the code to be copied into, and run `git clone <link-to-repo>`.
4. In VSCode, you can now view the code with **File > Open Folder**.
### Setting up Conda
1. We now need to use Conda to install the required dependencies for ORCA.
2. Before we create the conda environment, check that **GCC** is installed by running `gcc --version` in the WSL terminal. If the gcc command is not found, install it with `sudo apt update && sudo apt install gcc`.
3. Navigate to the folder containing the code. If you are using a Raspberry Pi to control the SDR, then use `environment-rpi.yaml` in the next step instead. Run `conda env create -f environment.yaml`.
4. Once the environment is installed, activate it with `conda activate uhd`.
5. Finally, we need to install any remaining dependencies for compiling the code, such as `make`. To install them, run `sudo apt update && sudo apt install build-essential`.

#### **The code is now installed and ready to be compiled and run, either manually or using the `run.py` script provided.**
