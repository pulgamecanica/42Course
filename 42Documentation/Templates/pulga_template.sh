#!/bin/bash

C_BLACK='\033[0;30m'
C_RED='\033[0;31m'
C_GREEN='\033[0;32m'
C_YELLOW='\033[0;33m'
C_BLUE='\033[0;34m'
C_WHITE='\033[0;37m'
C_END='\033[0m'

unset_on_exit() {
    # unset $(set | grep "^VARIABLE_PREFIX" | cut -d "=" -f 1)
    unset SOURCES_PATH
    unset INCLUDES_PATH
    unset QUIET_MAKEFILE
    unset CPP_VERSION
    unset DATE
    unset PROJECT
    unset COPY_CONSTRUCTOR
    unset ASSIGN_OPERATOR
    unset OVERLOADING
    unset DEFAULTING
    unset PARAMS
    unset VERBOSE
    unset CLASS
    unset NAMESPACE
    unset IFS
    unset SRC_PATH
    unset INC_PATH
    unset CLASS_HPP
    unset CLASS_CPP
    unset LINE
    unset line_number
    unset MAIN_PATH
    unset HEADER
    unset PARAMS_KEYS
    unset PARAMS_VALUES
}

usage() {
    echo "Usage: $0 {new|class} [options] [name]" >&2
    echo "Options for new:"
    echo "  -s, --sources-path <path>    Specify sources path (default: ./srcs)"
    echo "  -i, --includes-path <path>   Specify includes path (default: ./includes)"
    echo "  -m, --main-path [<path>]     Specify main path (default: --sources-path), not providing a path will fallback to the root ./"
    echo "  -q, --quiet                  Enable quiet mode on Makefile"
    echo "  -v, --version [version]      Specify c++ version (default: 98), Ex: -v 98 -v 11 -v 23"
    echo ""
    echo "Options for class:"
    echo "  The name of the class can contain a namespace Ex: namespace:className"
    echo "  -c, --copy-constructor       Generate copy constructor"
    echo "  -a, --assign-operator        Generate assign operator"
    echo "  -o, --overloading            Generate overloading operator (<<)"
    echo "  -d, --defaulting             Use default for the copy and assign if not true. (this feature is only for c++ version >= 11"
    echo "  -p, --params <param_list>    Specify parameters with types (e.g., param1:string param2:int), not specifing a type will fallback to std::string"
    echo ""
    echo "--verbose                      Enable verbose mode (default: false)"
    unset_on_exit
}

debug() {
    if [ "$VERBOSE" = true ]; then
        echo -e "${C_YELLOW}$1${C_END}"
    fi
}

remove_trailing_slash() {
    local path=$1
    if [ "${path: -1}" = "/" ]; then
        echo "${path%/}"
    else
        echo "$path"
    fi
}

cpp_new() {
    SOURCES_PATH="./srcs"
    INCLUDES_PATH="./includes"
    QUIET_MAKEFILE=false
    CPP_VERSION="98"
    DATE=`date`

    # Parse options
    while [[ $# -gt 0 ]]; do
        case "$1" in
            -s | --sources-path )
                if [[ -z $2 || $2 == -* ]]; then
                    echo "Option --sources-path requires a value."
                    usage
                    return
                fi
                SOURCES_PATH=$2
                shift 2
                ;;
            -i | --includes-path )
                if [[ -z $2 || $2 == -* ]]; then
                    echo "Option --includes-path requires a value."
                    usage
                    return
                fi
                INCLUDES_PATH=$2
                shift 2
                ;;
            -m | --main-path )
                if [[ -z $2 || $2 == -* ]]; then
                    MAIN_PATH="./"
                    shift
                else
                    MAIN_PATH=$2
                    shift 2
                fi
                ;;
            -q | --quiet )
                QUIET_MAKEFILE=true
                shift
                ;;
            -v | --version )
                if [[ -z $2 || $2 == -* ]]; then
                    echo "Option --version requires a value."
                    usage
                    return
                fi
                CPP_VERSION=$2
                shift 2
                ;;
            --verbose )
                VERBOSE=true
                shift
                ;;
            -- )
                shift
                break
                ;;
            * )
                if [[ $1 == -* ]]; then
                    usage
                    return
                    break
                fi
                if [ -z "$PROJECT" ]; then
                    PROJECT=$1
                    shift
                else
                    echo "Project has been already assigned: $PROJECT"
                    usage
                    return
                fi
                ;;
        esac
    done
    
    if [ -z "$MAIN_PATH" ]; then
        MAIN_PATH=$SOURCES_PATH
    fi
    
    # Normalize data, clients might do ./path/ or path or path/; etc
    SOURCES_PATH=$(remove_trailing_slash "$SOURCES_PATH")
    INCLUDES_PATH=$(remove_trailing_slash "$INCLUDES_PATH")
    MAIN_PATH=$(remove_trailing_slash "$MAIN_PATH")

    
    # If there are remaining arguments, assume the first one is the project name
    if [ -z "$PROJECT" ]; then
        echo "Enter Project Name..."
        read PROJECT
    fi

    # Debug messages
    debug "SOURCES_PATH: $SOURCES_PATH"
    debug "INCLUDES_PATH: $INCLUDES_PATH"
    debug "MAIN_PATH: $MAIN_PATH"
    debug "C++Version: C++$CPP_VERSION"
    debug "QUIET: $QUIET_MAKEFILE"
    debug "PROJECT: $PROJECT"
    printf "${C_RED}Creating C++ Project ${C_YELLOW}[$PROJECT]${C_END}\n"

    # Create Project Structure
    mkdir -p ${PROJECT} ${PROJECT}/${INCLUDES_PATH} ${PROJECT}/${SOURCES_PATH}
    touch ${PROJECT}/Makefile ${PROJECT}/${INCLUDES_PATH}/${PROJECT}.inc ${PROJECT}/${MAIN_PATH}/main.cpp

    # Makefile
    cat <<EOF > "${PROJECT}/Makefile"
# Makefile created automatically from script
# C++ ${PROJECT} [$DATE]
EOF
    if [ "$QUIET_MAKEFILE" = true ]; then
        echo "MAKEFLAGS += --silent" >> "${PROJECT}/Makefile"
    else
        echo "#MAKEFLAGS += --silent  # Silence makefile [Commented by default]" >> "${PROJECT}/Makefile"
    fi

    cat <<EOF >> "${PROJECT}/Makefile"
NAME            =   ${PROJECT}

NAME_DEBUG      =   \${NAME}_debug

INC_PATH        =   ${INCLUDES_PATH}

SRC_PATH        =   ${SOURCES_PATH}

SRC             =   ${MAIN_PATH}/main.cpp

CFLAGS          =   -Wall -Wextra -Werror -std=c++${CPP_VERSION} -pedantic -fsanitize=address #-O3 -g -fsanitize=leak

INC             =   -I\${INC_PATH}

OBJ_D           =   objs
OBJ_D_DEBUG     =   objs_debug

OBJ             =   \$(SRC:\${SRC_PATH}/%.cpp=\${OBJ_D}/%.o)
OBJ_DEBUG       =   \$(SRC:\${SRC_PATH}/%.cpp=\${OBJ_D_DEBUG}/%.o)

UNAME           :=  \$(shell uname)

RM              =   rm -rf

ifeq (\$(UNAME), Darwin)
    CC = c++
else ifeq (\$(UNAME), FreeBSD)
    CC = clang++
else
    CC  = g++
    CFLAGS += -D LINUX
endif

all:        \${NAME}

\${OBJ_D}/%.o:\${SRC_PATH}/%.cpp
			@mkdir -p \${OBJ_D}
			\$(CC) \$(CFLAGS) \${INC} -o \$@ -c \$<

\${NAME}:   \${OBJ}
			@printf "Compiling \$(C_YELLOW)\$(NAME)\$(C_END) ... \n"
			\$(CC) \$(CFLAGS) \${INC} \$(OBJ) -o \$(NAME)
			@printf "\$(C_GREEN)DONE\$(C_END)\n"

\${OBJ_D_DEBUG}/%.o:\${SRC_PATH}/%.cpp
			@mkdir -p \${OBJ_D_DEBUG}
			\${CC} \${CFLAGS} -D DEBUG=1 \${INC} -o \$@ -c \$<

\${NAME_DEBUG}: \$(OBJ_DEBUG)
			@printf "Compiling DEBUG \$(C_YELLOW)\$(NAME_DEBUG)\$(C_END) ...\n"
			\$(CC) \$(CFLAGS) \${INC} \$(OBJ_DEBUG) -o \$(NAME_DEBUG)
			@printf "\$(C_GREEN)DONE\$(C_END)\n"

debug:      \${NAME_DEBUG}

test:       re
			@printf "\$(C_BLUE)Testing \$(C_YELLOW)\$(NAME)\$(C_END)\n"
			@printf "\n\$(C_BLUE)********************************************\$(C_END)\n"
			@./\$(NAME)
			@printf "\n\$(C_BLUE)********************************************\$(C_END)\n"
			@printf "\n\$(C_BLUE)Finished Test...\$(C_END)\n"
			@\$(MAKE) show --no-print-directory
			@\$(MAKE) fclean --no-print-directory

valgrind:   re
			valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./\$(NAME) \$(NAME)_ARGS
			@echo "[MAKEFILE] You can setup test arguments by setting up the env \$(NAME)_ARGS"
			@echo "[MAKEFILE] Ex: export \$(NAME)_ARGS=\"-a --recursive ..\""

clean:
			\$(RM) \$(OBJ_D) \$(OBJ_D_DEBUG)
			@printf "\$(C_RED)Cleaning objs\$(C_END)\n"

fclean:     clean
			\$(RM) \$(NAME) \$(NAME_DEBUG)
			@printf "\$(C_RED)Deleted Everything\$(C_END)\n"

re: fclean all

show:
	@printf "\$(C_GREEN)"
	@printf "UNAME     : \$(UNAME)\n"
	@printf "NAME      : \$(C_YELLOW)\$(NAME)\$(C_GREEN)\n"
	@printf "CC        : \$(CC)\n"
	@printf "CFLAGS    : \$(CFLAGS)\n"
	@printf "INCLUDES  : \$(INC)\n"
	@printf "SRC       : \$(C_YELLOW)\$(SRC)\$(C_GREEN)\n"
	@printf "OBJ       : \$(C_YELLOW)[\$(OBJ_D)] --> \$(OBJ)\$(C_END)\n"

.PHONY: all test re

C_BLACK     = \033[0;30m
C_RED       = \033[0;31m
C_GREEN     = \033[0;32m
C_YELLOW    = \033[0;33m
C_BLUE      = \033[0;34m
C_WHITE     = \033[0;37m
C_END       = \033[0m

EOF
    printf "${C_GREEN}    create    ${C_END}${PROJECT}/Makefile\n"

    # main.cpp
    cat << EOF > ${PROJECT}/${MAIN_PATH}/main.cpp
//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "${PROJECT}.inc"

int main(void)
{
    if (DEBUG)
        std::cout << "Debug ON!" << std::endl;
    std::cout << "Hello Friend\nPulgamecanica greets you :D\n";
    return (0);
}
EOF
    printf "${C_GREEN}    create    ${C_END}${PROJECT}/${MAIN_PATH}/main.cpp\n"

    #header .inc
    HEADER=$(echo ${PROJECT} | tr '[:lower:]' '[:upper:]')
    cat << EOF > ${PROJECT}/${INCLUDES_PATH}/${PROJECT}.inc
//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __${HEADER}_HPP__
# define __${HEADER}_HPP__

#ifndef DEBUG
# define DEBUG 0
#endif

#include <iostream>


# define BLACK  "\033[0;30m"
# define RED    "\033[0;31m"
# define GREEN  "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE   "\033[0;34m"
# define WHITE  "\033[0;37m"
# define ENDC   "\033[0m"


#endif
EOF
    printf "${C_GREEN}    create    ${C_END}${PROJECT}/${INCLUDES_PATH}/${PROJECT}.inc\n"
    printf "${C_YELLOW}${PROJECT}${C_BLUE} succesfully created :D${C_END}\n"
    unset_on_exit
}

class_help() {
    echo "Sorry something went wrong... please make sure you are inside a cpp project..."
    echo "--------------------"
    echo "|- Project"
    echo "|  - <include_path>/"
    echo "|  | - project.hpp"
    echo "|  | - Object.hpp"
    echo "|  | - .......hpp"
    echo "|  - <srcs_path>/"
    echo "|  | - Object.cpp"
    echo "|  | - .......cpp"
    echo "--------------------"
    unset_on_exit
    return
}

cpp_class() {
    COPY_CONSTRUCTOR=false
    ASSIGN_OPERATOR=false
    OVERLOADING=false
    DEFAULTING=false
    PARAMS=""
    VERBOSE=false

    # Parse options
    while [[ $# -gt 0 ]]; do
        case "$1" in
            -c | --copy-constructor )
                COPY_CONSTRUCTOR=true
                shift
                ;;
            -a | --assign-operator )
                ASSIGN_OPERATOR=true
                shift
                ;;
            -o | --overloading )
                OVERLOADING=true
                shift
                ;;
            -d | --defaulting )
                DEFAULTING=true
                shift
                ;;
            -p | --params )
                if [[ -z $2 || $2 == -* ]]; then
                    echo "Option --params requires a value."
                    usage
                    return
                fi
                PARAMS=$2
                shift 2
                ;;
            --verbose )
                VERBOSE=true
                shift
                ;;
            -- )
                shift
                break
                ;;
            * )
                if [[ $1 == -* ]]; then
                    usage
                    return
                fi
                if [ -z "$CLASS" ]; then
                    CLASS=$1
                    shift
                else
                    echo "Class has been already assigned: $CLASS"
                    usage
                    return
                fi
                ;;
        esac
    done

    # If there are remaining arguments, assume the first one is the class name
    if [ -z "$CLASS" ]; then
        echo "Enter Class Name..."
        read CLASS
    fi

    # Check if class parameter contains a namespace
    if [[ "$CLASS" == *":"* ]]; then
        # Extract namespace and class name
        NAMESPACE="${CLASS%%:*}"
        CLASS="${CLASS##*:}"
    else
        # Set NAMESPACE to empty if no namespace is provided
        NAMESPACE=""
    fi

    # Parse parameters into associative array
    declare -a PARAMS_KEYS=()
    declare -a PARAMS_VALUES=()
    for param in $PARAMS; do
        IFS=':' read -r NAME TYPE <<< "$param"
        PARAMS_KEYS+=("$NAME")
        if [ "$TYPE" ]; then
            PARAMS_VALUES+=("$TYPE")
        else
            PARAMS_VALUES+="std::string"
        fi
    done

    # Parse Makefile src and inc and create paths to .cpp & .hpp
    if [ -f "Makefile" ]; then
        SRC_PATH=$(grep -m 1 'SRC_PATH' Makefile | sed 's/SRC_PATH[[:space:]]*=[[:space:]]*//')
        INC_PATH=$(grep -m 1 'INC_PATH' Makefile | sed 's/INC_PATH[[:space:]]*=[[:space:]]*//')
        CLASS_HPP=${INC_PATH}/${CLASS}.hpp
        CLASS_CPP=${SRC_PATH}/${CLASS}.cpp
    else
        echo "Couldn't find Makefile variables SRC_PATH and INC_PATH"
        echo "Make sure there is a Makefile and you have this two variables"
        class_help
        return
    fi

    # Debug messages for parameters
    debug "Parameters set:"
    for ((i=0; i<${#PARAMS_KEYS[@]}; i++)); do
        debug "  Param: ${PARAMS_KEYS[$i]}, Type: ${PARAMS_VALUES[$i]}"
    done

    # Debug messages
    debug "CLASS CPP: $CLASS_CPP"
    debug "CLASS HPP: $CLASS_HPP"
    debug "COPY_CONSTRUCTOR enabled: $COPY_CONSTRUCTOR"
    debug "ASSIGN_OPERATOR enabled: $ASSIGN_OPERATOR"
    debug "OVERLOADING enabled: $OVERLOADING"
    debug "CLASS: $CLASS"
    if [[ -n "$NAMESPACE" ]]; then
        debug "NAMESPACE: $NAMESPACE"
    fi

    # Check if the class source file already exists
    if [ -f "$CLASS_CPP" ]; then
        echo "Error: Class source file already exists: $CLASS_CPP"
        return
    fi

    # Check if the class header file already exists
    if [ -f "$CLASS_HPP" ]; then
        echo "Error: Class header file already exists: $CLASS_HPP"
        return
    fi

    # Create files structure
    {
        touch ${SRC_PATH}/${CLASS}.cpp && touch ${CLASS_HPP}
    } || {
        class_help
        return
    }

    # Class header (hpp)
    HEADERC=$(echo $CLASS | tr '[:lower:]' '[:upper:]')
    cat << EOF > ${CLASS_HPP}
//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __${HEADERC}_HPP__
# define __${HEADERC}_HPP__

#include <iostream>

EOF

    # Conditionally include the namespace
    if [[ -n "$NAMESPACE" ]]; then
        echo "namespace ${NAMESPACE} {" >> ${CLASS_HPP}
    fi

    # Class declaration
    cat << EOF >> ${CLASS_HPP}
class ${CLASS} {
    public:
        ${CLASS}();
        ~${CLASS}();
EOF

    # Conditionally include the copy constructor
    if [ "$COPY_CONSTRUCTOR" = true ]; then
        echo "        ${CLASS}(const ${CLASS}&);" >> ${CLASS_HPP}
    elif [ "$DEFAULTING" == true ]; then
        echo "        ${CLASS}(const ${CLASS}&);" >> ${CLASS_HPP}
    fi

    # Conditionally include the assignment operator
    if [ "$ASSIGN_OPERATOR" = true ]; then
        echo "        ${CLASS}& operator= (const ${CLASS}&);" >> ${CLASS_HPP}
    elif [ "$DEFAULTING" == true ]; then
        echo "        ${CLASS}& operator= (const ${CLASS}&) = default;" >> ${CLASS_HPP}
    fi

    # Add all parameters
    echo "    private:" >> ${CLASS_HPP}
    for ((i=0; i<${#PARAMS_KEYS[@]}; i++)); do
        echo "        ${PARAMS_VALUES[$i]} ${PARAMS_KEYS[$i]};" >> ${CLASS_HPP}
    done

    # Class declaration ending
    echo "};" >> ${CLASS_HPP}
    
    # Conditionally include the overloading operator
    if [ "$OVERLOADING" = true ]; then
        echo "std::ostream& operator<<(std::ostream&, const ${CLASS}&);" >> ${CLASS_HPP}
    fi

    # Conditionally include the namespace ending
    if [[ -n "$NAMESPACE" ]]; then
        echo "}" >> ${CLASS_HPP}
    fi

    # Class header end
    echo "#endif" >> ${CLASS_HPP}

    printf "${C_GREEN}    create    ${C_END}${CLASS_HPP}\n"

    # Class source (cpp)
    cat << EOF > ${CLASS_CPP}
//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "${CLASS}.hpp"

EOF
    
    # Conditionally include the namespace
    if [[ -n "$NAMESPACE" ]]; then
        echo "namespace ${NAMESPACE} {" >> ${CLASS_CPP}
    fi

    # Class definition
    cat << EOF >> ${CLASS_CPP}
${CLASS}::${CLASS}() {
$(for ((i=0; i<${#PARAMS_KEYS[@]}; i++)); do
    echo "	(void)${PARAMS_KEYS[$i]};"
done)
    // TODO (default constructor)
}

${CLASS}::~${CLASS}() {
    std::cout << "${CLASS}" << " destroyed" << std::endl;
    // TODO (destructor)
}

EOF

    # Conditionally include the copy constructor    
    if [ "$COPY_CONSTRUCTOR" = true ]; then
    cat << EOF >> ${CLASS_CPP}
${CLASS}::${CLASS}(const ${CLASS}& param) {
    // TODO (copy constructor)
    (void)param;
}

EOF
    fi

    # Conditionally include the assignment operator
    if [ "$ASSIGN_OPERATOR" = true ]; then
       cat << EOF >> ${CLASS_CPP}
${CLASS}& ${CLASS}::operator= (const ${CLASS}& param) {
    // TODO (Assignment operatior)
    // std::swap()
    (void)param;
    return (*this);
}

EOF

    fi

    # Conditionally include the overloading operator
    if [ "$OVERLOADING" = true ]; then
       cat << EOF >> ${CLASS_CPP}
std::ostream& operator<<(std::ostream& s, const ${CLASS}& param) {
    // s << param.CONST_METHOD()
    (void)param;
    return (s);
}

EOF
    fi

    # Conditionally include the namespace ending
    if [[ -n "$NAMESPACE" ]]; then
        echo "}" >> ${CLASS_CPP}
    fi

    printf "${C_GREEN}    create    ${C_END}${CLASS_CPP}\n"

    # Edit Makefile add new class

    # Define the line to be added
    LINE="		${CLASS_CPP} \\"

    # Find the line number where "SRC" followed by "=" appears
    line_number=$(grep -n "^SRC .*=.*$" Makefile | head -n 1 | cut -d ":" -f 1)
    line_number=$((line_number + 1))

    # Insert the line before the found line number
    { head -n "$((line_number - 1))" Makefile; echo "$LINE"; tail -n "+$line_number" Makefile; } > temp && mv temp Makefile

    # Check if the backslash is already present at the end of the line, if not, add it
    if ! grep -q "^SRC .*\\s*\\\\$" Makefile; then
        awk '/^SRC / {print $0 " \\"} !/^SRC / {print}' Makefile > temp && mv temp Makefile
    fi

    printf "${C_GREEN}    edit      ${C_END}Makefile\n"
    unset_on_exit
}
