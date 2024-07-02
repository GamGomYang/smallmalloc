# Homework 2: smalloc
### Course: 5118020-03 Operating Systems
### Due Date: 2024-05-14
### Students: 2022041052 Geumyoung Kim, 2022041046 Jaeyun Byeon

## Overview
- **Objective:** Complete `smalloc.c`, an in-house heap memory allocator for C programs.
- **Focus Point:** Memory management API: `mmap()`

## Contents
- **Operations**
- **Scenario 1:** `smalloc_mode`
- **Scenario 2:** `smalloc_mode`
- **Scenario 3:** `smalloc_coalesce`

## Operations
- `smalloc(size_t s)`
- `srealloc(void *p, size_t s)`
- `sfree(void *p)`
- `smalloc_mode(size_t s, smmode mode)`
- `smcoalesce()`

## Scenario 1: smalloc_mode
Let's consider this situation:
- `smalloc(3000)`
- `smalloc(2000)`
- `smalloc(4000)`
- `smalloc(1000)`

## Scenario 2: smalloc_mode
Let's consider this situation:
- `smalloc(3000)`
- `smalloc(2000)`
- `smalloc(4000)`
- `smalloc(1000)`
- `sfree(3000)`

## Scenario 3: smalloc_coalesce
Let's consider this situation:
- `smalloc(1000)`
- `smalloc(1000)`
- `smalloc(1000)`
- `smalloc(3000)`
- `srealloc(500)`
- `srealloc(1500)`
- `sfree()`
- `smcoalesce()`
