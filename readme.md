# Setting up a Python Virtual Environment

This script will guide you through setting up a virtual environment for your Python project.

## Prerequisites:

* **Python 3:** Ensure you have Python 3.11+ installed on your system. 
* **Python 3 venv module:**  This should be included in your Python 3.11 installation.
* **G++**: For compiling C++ scripts
* **CPPYY:** For running C++ scripts on Python

## Steps (for Debian):

0. **Install python3-dev, python virtual environment, g++, and cmake. (Probably need sudo)**
    ```bash
    apt update
    apt upgrade
    apt install python3-dev
    apt install python3-venv
    apt install g++
    apt install cmake
1. **Create the virtual environment:**
   ```bash
   python3 -m venv my-venv
2. **Switch to the virtual environment**
    ```bash
    source my-venv/bin/activate
3. **Upgrade the pip**
    ```bash
    pip install --upgrade pip
4. **Install Jupyter Notebook (Optional if you use Visual Studio Code)**
    ```bash
    pip install notebook
5. **Install cppyy**
    ```bash
    pip install cppyy
6. **Install matplotlib library**
    ```bash
    pip install matplotlib
7. **Install ipywidgets library**
    ```bash
    pip install ipywidgets
8. **Install ipykernel for the notebook**
    ```bash
    pip install ipykernel