# City Manager Project

Current stage: All phases completed

## Short description

This project is a UNIX/C application for managing city infrastructure reports. It works with districts, reports, roles, binary files, permissions, logs, symbolic links, processes, signals, pipes and redirects.

The main executables are:

- city_manager
- monitor_reports
- city_hub
- calculate_scores

## How to compile

Compile everything with:
./compileAll.sh

## Phase 1 - city_manager

The main program is city_manager. It can add, list, view, filter and remove reports from district folders.

General format:

./city_manager --role <manager|inspector> --user <username> <command> <district> [extra_arguments]

Examples:

./city_manager --role inspector --user bob --add downtown

./city_manager --role inspector --user bob --list downtown

./city_manager --role inspector --user bob --view downtown 1

./city_manager --role inspector --user bob --filter downtown "severity:>=:2"

./city_manager --role inspector --user bob --filter downtown "severity:>=:2" "category:==:road"

Manager-only examples:

./city_manager --role manager --user alice --remove_report downtown 1

./city_manager --role manager --user alice --update_threshold downtown 3

The filter arguments should be written between quotes because the shell can interpret < and > as redirection operators.

Each district has its own folder with:

- reports.dat
- district.cfg
- logged_district

The program also creates active_reports symbolic links for the report files.

## Phase 2 - monitor_reports

Phase 2 adds the monitor program.

Run it with:
./monitor_reports

The monitor creates a .monitor_pid file, stores its PID there and waits for signals.
When a new report is added with city_manager, the monitor is notified using SIGUSR1.

Example:
./city_manager --role inspector --user bob --add downtown

If the monitor is running, it prints a message saying that a new report was added. If the monitor is not running, city_manager writes in the district log that the monitor could not be informed.

Phase 2 also adds remove_district, which is manager-only:

./city_manager --role manager --user alice --remove_district downtown

This removes the district folder and the corresponding active_reports symbolic link.

## Phase 3 - city_hub and calculate_scores

Phase 3 adds the interactive hub.

Run it with:
./city_hub

Available commands inside city_hub:

a. start_monitor
b. calculate_scores <district1> <district2> ...
c. help
d. exit

Example:
calculate_scores downtown First SecondDistrict

The hub starts a separate scorer process for each district. The output of each scorer is redirected through a pipe using dup2, and city_hub prints the results together.

The scorer program can also be tested directly:

./calculate_scores downtown

It reads the reports.dat file from that district and computes the workload score for every inspector. The score is the sum of all severity values from reports written by that inspector.

Example output:

District: downtown
inspector=alice;score=2
inspector=bob;score=5

## Example test flow

Compile everything:

./compileAll.sh

Add reports:

./city_manager --role inspector --user bob --add downtown
./city_manager --role manager --user alice --add downtown

List reports:
./city_manager --role inspector --user bob --list downtown

Filter reports:
./city_manager --role inspector --user bob --filter downtown "severity:>=:2"

Start the hub:
./city_hub

Inside city_hub:
start_monitor
calculate_scores downtown First SecondDistrict
exit

## Notes

The project uses system calls such as open, close, read, write, lseek, ftruncate, stat, lstat, mkdir, symlink, unlink, chmod, fork, exec, pipe, dup2, kill and sigaction.

Reports are stored as fixed-size binary records in reports.dat.

The project was tested with multiple districts and multiple reports.
