# TCP Optimizer

TCP Optimizer or WIN-OPTI (name is work in progess) is a command-line tool designed to optimize TCP settings and download speeds of applications on Windows systems for improved network performance. It provides features for tuning various TCP parameters, managing application bandwidth usage, and conducting network speed tests.

This project is being developed as a capstone project for 2023-2024, and is still in the works, many of the features listed below are not done or still in progess. For now what is below is a long term goal for the finalized version of this app.

## Table of Contents

- [Features](#features)
- [How it works](#how-it-works)
- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Contributing](#contributing)

## Features
### Auto TCP Optimization

TCP Optimizer offers automatic optimization of TCP settings by testing different setting configurations and selecting the optimal values for maximum network throughput. It conducts speed tests and adjusts parameters such as TCP window size, congestion control algorithms, and receive-side scaling (RSS) options.

### Manual Configuration

For users who prefer fine-tuned control over TCP settings, TCP Optimizer provides the ability to manually tweak individual parameters. It again allows customization of parameters such as TCP window scaling, congestion control provider, ECN (Explicit Congestion Notification) capability, and more. (WORK IN PROGESS)

### Bandwidth Management

TCP Optimizer includes functionality manage application bandwidth usage. It tracks the network usage of running applications and applies Quality of Service (QoS) policies to prioritize or limit bandwidth for specific applications based on user defined criteria.

### Backup and Restore

To ensure system stability and easy rollback of changes, TCP Optimizer supports backup and restore functionality for TCP settings. Users can create backups of current TCP configurations and restore them later if needed, making it convenient to revert to previous settings. (WORK IN PROGESS)

### User-friendly Interface

A simple to use command-line interface is used to navigate TCP Optimizer, it offers a straightforward user experience. Users can navigate through menu options easily, execute tasks, and view status messages and results.

### Key Features:

- **Bandwidth Management:** Allows users to manage bandwidth usage among running applications, user selected applications receive sufficient network resources during high-demand scenarios.
  
- **Dynamic Optimization:** Dynamic adjusting of QoS policys based on real-time network conditions (WORK IN PROGESS)

- **Application Prioritization:** Users can prioritize specific applications to ensure they receive preferential treatment in terms of windows provied CPU time. This feature is particularly useful for ensuring smooth performance in applications such as streaming services.

## How It Works

TCP Optimizer is designed to work optimally when multiple applications are running simultaneously on a Windows system. It is particularly effective in adjusting bandwitdh limiting to prioritize certain applications utilizeing the [windows QoS](https://learn.microsoft.com/en-us/windows-server/networking/technologies/qos/qos-policy-top) sytem to limit apps bandwitdh along with the aplication priority system that windows already has in place.
### Benefits:

- **Improved Performance:** Enhances the performance of critical applications by ensuring they receive sufficient network resources, even when the system is under heavy load.
  
- **Network Stability:** By dynamically optimizing settings, TCP Optimizer helps maintain network stability and reduces the likelihood of congestion-related issues, such as packet loss or latency spikes.

- **Customization:** Users have the flexibility to customize TCP settings according to their specific requirements, allowing for fine-tuning of settings based on individual preferences and usage patterns.
  
## Installation

To use TCP Optimizer, follow these steps:

1. **Clone Repository**: Clone the TCP Optimizer repository from [GitHub](https://github.com/powplowdevs/Capstone-optimizer) to your local machine.

2. **Build Binary**: Compile the source code using a compatible C++ compiler. I would use the command ```g++ .\editor.cpp .\CLI.cpp -o main```. Ensure that you have the necessary dependencies installed, including the WinAPI headers for Windows development.

3. **Run Executable**: Execute the compiled binary file (main.exe) or whatever you named it from the command line to launch the TCP Optimizer CLI.

## Usage

Once TCP Optimizer is running, you can use the following commands to perform various tasks:

- `1. Run Speed Test`: Conduct a network speed test to measure upload and download speeds.
- `2. Auto Optimize`: Automatically optimize TCP settings for maximum network performance.
- `3. Manual Configuration`: Manually tweak TCP parameters based on user preferences.
- `4. Bandwidth Management`: Monitor and manage application bandwidth usage.
- `5. Backup and Restore`: Create backups of TCP settings and restore them if needed.
- `6. Help`: View additional information about TCP Optimizer and its commands.
- `7. Exit`: Close TCP Optimizer and exit the application.

## Configuration

TCP Optimizer relies on the manipulation of Windows registry settings and system APIs to modify TCP parameters. It uses various WinAPI functions for tasks such as querying running processes, adjusting TCP settings, and applying QoS policies. for this reason it need to be run in administrator mode.

## Contributing

Contributions to TCP Optimizer are welcome! If you encounter any bugs, have suggestions for new features, or would like to contribute improvements to the project, please open an issue on GitHub or submit a pull request with your changes.

<hr>

<h3>Dev stuff</h3>

Todo: https://trello.com/b/BvJY8htR/main-board
