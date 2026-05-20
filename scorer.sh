#!/bin/bash

./city_manager --role manager --user admin --list $1 | grep -e Inspector: -e Severity