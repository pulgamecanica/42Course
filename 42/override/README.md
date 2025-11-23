# Override - Binary Exploitation CTF

A collection of binary exploitation challenges focusing on various vulnerabilities and exploitation techniques. Each level contains a SUID binary that must be exploited to escalate privileges and retrieve the password for the next level.

## Project Structure

Each level directory contains:
- **source.c**: Reconstructed C code mimicking the binary's behavior
- **walkthrough.md**: Complete documentation including vulnerability analysis, exploitation steps, and prevention methods
- **Ressources/**: Binary files and helper scripts for testing
- **flag**: Password for the next level

## How to Use

1. **Connect to the VM**: `ssh level00@localhost -p 4242`
2. **Analyze the binary**: Use tools like GDB, objdump, or Ghidra
3. **Develop exploit**: Test locally using files in Ressources/
4. **Execute exploit**: Gain access and retrieve the password from `/home/users/levelXX/.pass`

## Level Overview

### Level 0 - Hardcoded Password
- **Vulnerability**: Hardcoded Credentials ([CWE-798](https://cwe.mitre.org/data/definitions/798.html))
- **Technique**: Reverse engineering to extract hardcoded password
- **Key Learning**: Never embed credentials in binaries

### Level 1 - Buffer Overflow
- **Vulnerability**: Stack-Based Buffer Overflow ([CWE-121](https://cwe.mitre.org/data/definitions/121.html))
- **Technique**: Overwrite return address with shellcode
- **Key Learning**: Proper bounds checking with buffer operations

### Level 2
- **Vulnerability**: TBD
- **Technique**: TBD
- **Key Learning**: TBD

### Level 3
- **Vulnerability**: TBD
- **Technique**: TBD
- **Key Learning**: TBD

### Level 4
- **Vulnerability**: TBD
- **Technique**: TBD
- **Key Learning**: TBD

### Level 5
- **Vulnerability**: TBD
- **Technique**: TBD
- **Key Learning**: TBD

### Level 6
- **Vulnerability**: TBD
- **Technique**: TBD
- **Key Learning**: TBD

### Level 7
- **Vulnerability**: TBD
- **Technique**: TBD
- **Key Learning**: TBD

### Level 8
- **Vulnerability**: TBD
- **Technique**: TBD
- **Key Learning**: TBD

### Level 9
- **Vulnerability**: TBD
- **Technique**: TBD
- **Key Learning**: TBD

## Resources

- [CWE - Common Weakness Enumeration](https://cwe.mitre.org/)
- [OWASP Top 10](https://owasp.org/www-project-top-ten/)
- [Exploit Education](https://exploit.education/)
- [www.ajulien.fr/linux-shellcodes-x86-x86_64/](www.ajulien.fr/linux-shellcodes-x86-x86_64/)

## VM Access

- **SSH Port**: 4242
- **Users**: level00-level09
- **Password Location**: `/home/users/levelXX/.pass`

