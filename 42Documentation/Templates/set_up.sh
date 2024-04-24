#!/bin/bash

#EXISTS_ZS=$(ls ~/.zshrc | grep -v "No such file" | wc -l)
#EXISTS_BP=$(ls ~/.bash_profile | grep -v "No such file" | wc -l)
NEED_ZS=$(grep "~/.pulga_template.sh" ~/.zshrc | wc -l)
NEED_BP=$(grep "~/.pulga_template.sh" ~/.bash_profile | wc -l)

rm ~/.pulga_template.sh
cp pulga_template.sh ~/.pulga_template.sh
chmod 777 ~/.pulga_template.sh
if [[ $((NEED_ZS)) -eq 0  || $((NEED_BP)) -eq 0 ]]; then
#if [ $((EXISTS_ZS)) -eq 0 ]; then
echo "source ~/.pulga_template.sh" >> ~/.zshrc
#fi
#if [ $((EXISTS_BP)) -eq 0 ]; then
echo "source ~/.pulga_template.sh" >> ~/.bash_profile
#fi
#if [ $((EXISTS_BC)) -eq 0 ]; then
echo "source ~/.pulga_template.sh" >> ~/.bashrc 
#fi
echo "commands:"
echo "+    cpp_new [options] [name]"
echo "+    cpp_class [options] [[namespace:]name]"
echo "Please restart your terminal to set up your new commands..."
echo "Visit: https://github.com/pulgamecanica/42Course/tree/main/42Documentation/Templates"
echo "  To see some examples and alias which can help you a lot!"
else
echo "You already have this command installed... Updating files just in case"
fi
