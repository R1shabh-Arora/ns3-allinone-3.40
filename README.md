
# ns-3 Network Simulation Project

This project contains a network simulation setup using ns-3, a discrete-event network simulator for internet systems. The provided simulation script (`Test1.cc`) configures a network with 50 nodes, supports multiple routing protocols, simulates UDP traffic, and captures performance metrics.

## Table of Contents
- [Introduction](#introduction)
- [Requirements](#requirements)
- [Installation](#installation)
- [Running the Simulation](#running-the-simulation)
- [Simulation Details](#simulation-details)
- [Output Files](#output-files)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)

## Introduction

This is **_ns-3-allinone_**, a repository with some scripts to download and build the core components around the [ns-3 network simulator](https://www.nsnam.org). More information about this can be found in the [ns-3 tutorial](https://www.nsnam.org/documentation/).

If you have downloaded this in tarball release format, this directory contains some released ns-3 version, along with the repository for the [NetAnim network animator](https://gitlab.com/nsnam/netanim/). In this case, just run the script `build.py`, which attempts to build NetAnim (if dependencies are met) and then ns-3 itself. If you want to build ns-3 examples and tests (a full ns-3 build), instead type:
```bash
./build.py --enable-examples --enable-tests
```
or you can simply enter into the ns-3 directory directly and use the build tools therein (see the tutorial).

This directory also contains the [bake build tool](https://www.gitlab.com/nsnam/bake/), which allows access to other extensions of ns-3, including the Direct Code Execution environment, BRITE, click and openflow extensions for ns-3. Consult the ns-3 tutorial on how to use bake to access optional ns-3 components.

If you have downloaded this from Git, the `download.py` script can be used to download bake, netanim, and ns-3-dev. The usage to use basic ns-3 (netanim and ns-3-dev) is to type:
```bash
./download.py
./build.py --enable-examples --enable-tests
```
and change directory to ns-3-dev for further work.

## Requirements
- Ubuntu 20.04 or later
- C++ compiler (g++)
- Python 3.x
- Git

## Installation

### 1. Install Dependencies
```bash
sudo apt-get update
sudo apt-get install g++ python3 python3-pip git
```

### 2. Clone the ns-3 Repository
```bash
git clone https://gitlab.com/nsnam/ns-3-dev.git
cd ns-3-dev
```

### 3. Download the ns-3 Dependencies
```bash
./build.py --enable-examples --enable-tests
```

### 4. Build ns-3
```bash
./build.py
```

### 5. Verify Installation
```bash
./build.py --run hello-simulator
```
If the installation is successful, you should see the output: "Hello Simulator".

### 6. Clone the Project Repository
```bash
git clone https://github.com/yourusername/ns3-network-simulation.git
cd ns3-network-simulation
```

## Running the Simulation

1. **Navigate to the ns-3 directory:**
    ```bash
    cd ~/ns-3-dev
    ```

2. **Copy the Simulation Script to the ns-3 Scratch Directory:**
    ```bash
    cp ~/ns3-network-simulation/data/Test1.cc ~/ns-3-dev/scratch/
    ```

3. **Run the Simulation:**
    ```bash
    ./ns3 run scratch/Test1.cc
    ```

## Simulation Details

The `Test1.cc` script configures a network with the following parameters:
- **Nodes:** 50
- **Mobility Model:** RandomWaypointMobilityModel with a speed of 20 m/s and no pause time within a 300x1500 m region
- **WiFi Configuration:** Ad hoc mode with a 2 Mb/s rate (802.11b) and Friis loss model, transmit power set to 7.5 dBm
- **Routing Protocols:** Supports OLSR (default), AODV, and DSDV
- **Traffic:** 10 source/sink data pairs sending UDP data at 2.048 Kb/s each

## Output Files

The simulation outputs the following:
- **Standard Output:** Packet receptions with timestamps, node IDs, and source addresses
- **CSV File:** Data reception statistics tabulated each second
- **XML Files:** Tracing and flow monitor configuration data for analysis

## Project Structure

```
data/
    analyze_and_visualize.py      # Script to analyze and visualize captured data
    Test1.cc                      # Main ns-3 simulation script
    start_capture.sh              # Shell script to start packet capture and analysis
    index.html                    # Web dashboard for displaying network traffic analytics
    image.png                     # Example image or related visual asset
    README.txt                    # Project documentation
```

## Contributing

Feel free to submit pull requests or report issues to improve this project. Contributions are welcome!

