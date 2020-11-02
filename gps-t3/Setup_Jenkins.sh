#! /bin/bash
echo "Nottingham Trent University"
echo "This script will deploy the environment for SDI and SE"
echo "Author: Pedro Machado, pedro.baptistamachado@ntu.ac.uk"
sudo apt update
sudo apt upgrade -y
sudo apt dist-upgrade -y
sudo apt autoremove -y
sudo apt autoclean -y
sudo apt install build-essential cmake make libboost-test-dev ssh-askpass -y
echo "Finished!"
