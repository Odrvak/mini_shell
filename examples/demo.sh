#!/bin/bash

# Mini Shell Demo Script
# This script demonstrates various features of mini-shell

echo "========================================="
echo "   Mini Shell Feature Demonstration"
echo "========================================="
echo ""

# Test 1: Basic commands
echo "Test 1: Basic Commands"
echo "----------------------"
echo "Running: pwd"
pwd
echo ""
echo "Running: ls -l"
ls -l
echo ""

# Test 2: Output redirection
echo "Test 2: Output Redirection"
echo "--------------------------"
echo "Running: echo 'Hello World' > test.txt"
echo 'Hello World' > test.txt
echo "Running: cat test.txt"
cat test.txt
echo ""

# Test 3: Append redirection
echo "Test 3: Append Redirection"
echo "--------------------------"
echo "Running: echo 'Second line' >> test.txt"
echo 'Second line' >> test.txt
echo "Running: cat test.txt"
cat test.txt
echo ""

# Test 4: Input redirection
echo "Test 4: Input Redirection"
echo "-------------------------"
echo "Running: wc -l < test.txt"
wc -l < test.txt
echo ""

# Test 5: Background process
echo "Test 5: Background Process"
echo "--------------------------"
echo "Running: sleep 2 &"
sleep 2 &
echo "Process started in background"
echo ""

# Cleanup
echo "Cleaning up..."
rm -f test.txt

echo "========================================="
echo "   Demonstration Complete!"
echo "========================================="
