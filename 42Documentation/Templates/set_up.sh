#!/bin/bash

#EXISTS_ZS=$(ls ~/.zshrc | grep -v "No such file" | wc -l)
#EXISTS_BP=$(ls ~/.bash_profile | grep -v "No such file" | wc -l)
NEED_ZS=$(grep "~/.cpp_temp.sh" ~/.zshrc | wc -l)
NEED_BP=$(grep "~/.cpp_temp.sh" ~/.bash_profile | wc -l)

if [[ $((NEED_ZS)) -eq 0  || $((NEED_BP)) -eq 0 ]]; then
chmod 777 cpp_temp.sh
cp cpp_temp.sh ~/.cpp_temp.sh
#if [ $((EXISTS_ZS)) -eq 0 ]; then
echo "source ~/.cpp_temp.sh" >> ~/.zshrc
#fi
#if [ $((EXISTS_BP)) -eq 0 ]; then
echo "source ~/.cpp_temp.sh" >> ~/.bash_profile
#fi
echo "commands:"
echo "+    cpp_new   NAME"
echo "+    cpp_class CLASS"
echo "Please restart your terminal to set up your new commands..."
else
echo "You already have this command installed..."
fi
