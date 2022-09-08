#!/bin/bash

read name

tr -cs '[:alnum:]' '[\n*]' < "$name".txt | sort | uniq -c | sort -nr | head  -3
