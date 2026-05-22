#!/bin/bash

gcc -Wall -Wextra -o city_manager city_manager.c utils/commands.c utils/fileFunctions.c utils/inputValidation.c utils/report.c
gcc -Wall -Wextra -o monitor_reports monitor_reports.c
gcc -Wall -Wextra -o calculate_scores calculate_scores.c
gcc -Wall -Wextra -o city_hub city_hub.c

echo "Compiled all."