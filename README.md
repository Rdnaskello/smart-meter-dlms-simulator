# smart-meter-simulator

## Smart Meter Simulator with DLMS/COSEM Protocol Support

### Repository Overview
This project implements a software-based smart electricity meter simulator:

- **Protocol:** DLMS/COSEM (IEC 62056)
- **Core:** C++ using [Gurux.DLMS.cpp](https://github.com/gurux/gurux.dlms.cpp)
- **Platform:** Linux

---

## Installation & Build Instructions

### Step 1: Environment Preparation
1. Ensure Linux OS (e.g., Ubuntu) is installed.
2. Install required tools:
   ```bash
   sudo apt-get update
   sudo apt-get install build-essential cmake
   ```
3. Clone this repository:
   ```bash
   git clone https://github.com/your-username/smart-meter-simulator.git
   cd smart-meter-simulator
   ```
4. Install the Gurux.DLMS.cpp library:  
   Follow: [https://github.com/gurux/gurux.dlms.cpp](https://github.com/gurux/gurux.dlms.cpp)

### Step 2: Build the Project
```bash
mkdir build
cd build
cmake ..
make
```
> The executable (e.g., `smart_meter_simulator`) will be in the `build` folder.

---

## Running the Simulator

### Basic Run
```bash
./smart_meter_simulator
```

### Command-Line Parameters
- `-p [port]`: TCP port (default: `4060`)
- `-n [number]`: Number of meter instances (default: `1`)
- `-t [level]`: Trace level (`Error`, `Warning`, `Info`, `Verbose`, `Off`)

**Example:**
```bash
./smart_meter_simulator -p 5000 -n 3 -t Verbose
```

---

## Client Example

Use `smart_meter_client` to connect:
```bash
./smart_meter_client -h localhost -p 4060 -i WRAPPER -w 1 -c 16 -s 1 -r LN -g "1.0.1.8.0.255:2"
```

List all objects:
```bash
./smart_meter_client -h localhost -p 4060 -i WRAPPER -w 1 -c 16 -s 1 -r LN
```

---

## Functionality

- Access real-time register values
- Query internal clock
- Retrieve historical data (`ProfileGeneric`)

---

## Common Issues

| Error Message            | Cause                               | Solution                                      |
|--------------------------|--------------------------------------|-----------------------------------------------|
| `Connect failed`         | Server not running / wrong port     | Start simulator, verify port                 |
| `No response from server`| Network issues / simulator stopped  | Restart simulator, check network             |
| `Port already in use`    | Port occupied                       | Choose another port using `-p`               |
| `Invalid Authentication` | Invalid authentication level        | Adjust client parameters                     |
| `Segmentation fault`     | Bad command-line / memory issue     | Enable verbose mode `-t Verbose`             |

---

## Project Customization

### Add OBIS Objects
- Edit: `ObisObjectModel.cpp`
- Define new object with OBIS, type, default value
- Register it via `AddBasicTestObisObjects()`

### Modify Data Generation
- Edit: `DataGenerator.cpp`
- Implement new algorithms or randomization logic

### Configure Profiles
- Edit: `ProfileGenericManager.cpp`
- Adjust capture frequency, format, or storage path

### Port & Address Settings
- Via command-line or edit `main.cpp` / `DlmsHandler.cpp`

---

## Gurux Library

This simulator uses [Gurux.DLMS](https://github.com/gurux/gurux.dlms.cpp) to implement:
- COSEM object modeling
- DLMS protocol state machine
- AARE/AARQ exchange, get/set/action handling

---

## Student Info

- **Name:** Herbei Oleksandr  
- **Group:** KI-401  
- **Student ID:** 03

> This README is part of the Bachelor's thesis project: *"Software Simulator of a Smart Electricity Meter with DLMS/COSEM Support"*
