# Proiect-Sisteme-de-Operare-SO-
Proiect Sisteme de Operare, semestrul 2, anul 2, Boscu Tudor

Current stage: Phase 1 completed. 

# How to use:

- Compile the project: ./compileProject.sh (shellScript for compiling)

- How to run: ./city_manager --role <manager|inspector> --user <username> <command> <district> [extra_arguments]

Commands that I used to test:
./p --role manager --user userIdk --add First
./p --role inspector --user userIdk --list First
./p --role inspector --user userIdk --view First 1
./p --role manager --user userIdk --remove_report First 1
./p --role manager --user userIdk --update_threshold First 2
./p --role inspector --user userIdk --filter First "severity:>=:2" "category:==:road"