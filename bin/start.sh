#!/bin/bash
insmod $(dirname $0)/mod/transfer.ko
insmod $(dirname $0)/mod/probe.ko
insmod $(dirname $0)/mod/execve.ko
$(dirname $0)/hook_execve $1