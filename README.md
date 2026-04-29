# Proiect-Sisteme-de-Operare-SO-
Proiect Sisteme de Operare, semestrul 2, anul 2, Boscu Tudor

Current stage: Phase 1 completed. 

# How to use:

- Compile the project: ./compileProject.sh (shellScript for compiling)

- How to run: ./city_manager --role <manager|inspector> --user <username> <command> <district> [extra_arguments]

Commands that I used to test:

- ./city_manager --role manager --user userIdk --add <<districtName>>
- ./city_manager --role inspector --user userIdk --list <<districtName>>
- ./city_manager --role inspector --user userIdk --view <<districtName>> 1
- ./city_manager --role manager --user userIdk --remove_report <<districtName>> 1
- ./city_manager --role manager --user userIdk --update_threshold <<districtName>> 2
- ./city_manager --role inspector --user userIdk --filter <<districtName>> "severity:>=:2" "category:==:road"
- ./city_manager --role inspector --user userIdk --remove_district <<districtName>>