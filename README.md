# AP Assignment 7.2 - Football Fantasy Management System

This repository contains the implementation of a **Football Fantasy Management System**, developed as part of an Advanced Programming course. The project showcases object-oriented design principles and efficient coding practices to implement a command-line-based simulation of a fantasy football manager game.

## Project Overview

The system allows users to:
- Build and manage fantasy football teams.
- Work within budget constraints to buy and sell players.
- Assign roles and captains to players.
- Track team performance and rankings across simulated weeks.
- Query player statistics, team formations, and league standings.

The implementation includes robust error handling to manage scenarios such as invalid commands, insufficient budget, or unavailable players.

## Technical Highlights

1. **Object-Oriented Design**:
   - Polymorphism is used to handle different player roles (e.g., Goalkeepers, Defenders, Midfielders, Strikers).
   - Encapsulation ensures separation of concerns, with clearly defined classes for teams, players, and leagues.
   - Inheritance is applied to streamline role-specific behaviors of players.

2. **File Handling**:
   - Player and team data are read from CSV files, making the system extensible and easy to initialize with real-world datasets.
   - Weekly statistics and match results are dynamically processed to update team and league standings.

3. **Command-Based System**:
   - The program supports structured input commands to interact with the system, such as:
     - `POST buy_player`, `GET team_of_the_week`, `GET squad`, etc.
   - Commands are validated and provide meaningful feedback to users, such as budget status or errors (e.g., "Player not available").

4. **Multi-File Code Organization**:
   - The project is modular, splitting responsibilities across well-structured files.
   - A Makefile is included to streamline compilation and dependency management.

5. **Coding Style**:
   - Descriptive variable and function names enhance code readability.
   - Consistent indentation and comments clarify complex logic.
   - Efficient algorithms are used for processing queries, ensuring scalability.

## Learning Outcomes

This project demonstrates expertise in:
- Designing and implementing real-world applications using modern C++.
- Applying advanced programming techniques, including polymorphism and dynamic memory management.
- Building systems with a focus on maintainability and scalability.
- Handling structured data input and output through file integration.

The repository serves as a showcase of advanced programming and problem-solving skills, tailored for practical scenarios like game simulation and resource management.
