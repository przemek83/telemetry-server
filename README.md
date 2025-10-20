[![Build & test](https://github.com/przemek83/telemetry-server/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/przemek83/telemetry-server/actions/workflows/build-and-test.yml)
[![CodeQL](https://github.com/przemek83/telemetry-server/actions/workflows/github-code-scanning/codeql/badge.svg)](https://github.com/przemek83/telemetry-server/actions/workflows/github-code-scanning/codeql)
[![codecov](https://codecov.io/gh/przemek83/telemetry-server/graph/badge.svg?token=92ZWCFCNBQ)](https://codecov.io/gh/przemek83/telemetry-server)

[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=przemek83_telemetry-server&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=przemek83_telemetry-server)
[![Bugs](https://sonarcloud.io/api/project_badges/measure?project=przemek83_telemetry-server&metric=bugs)](https://sonarcloud.io/summary/new_code?id=przemek83_telemetry-server)
[![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=przemek83_telemetry-server&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=przemek83_telemetry-server)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=przemek83_telemetry-server&metric=coverage)](https://sonarcloud.io/summary/new_code?id=przemek83_telemetry-server)
[![Duplicated Lines (%)](https://sonarcloud.io/api/project_badges/measure?project=przemek83_telemetry-server&metric=duplicated_lines_density)](https://sonarcloud.io/summary/new_code?id=przemek83_telemetry-server)

# Table of content
- [About project](#about-project)
- [Building](#building)
   * [Used tools and libraries for the C++ part](#used-tools-and-libraries-for-the-c-part)
- [Usage](#usage)
   * [API Endpoints](#api-endpoints)
   * [Request Examples](#request-examples)
- [Configuration](#configuration)
   * [Server Settings](#server-settings)
   * [Logging](#logging)
- [Architecture](#architecture)
   * [Components](#components)
   * [Data Flow](#data-flow)
- [Licensing](#licensing)
- [Testing](#testing)

# About project
A lightweight C++ telemetry server for collecting and analyzing event data with HTTP API endpoints. The server provides a simple REST API for submitting telemetry data and querying statistical metrics with time-based filtering capabilities.

The implementation is written in C++17, with a focus on performance, thread-safety, and ease of integration. The server uses httplib for HTTP handling and nlohmann/json for JSON processing, making it suitable for real-time telemetry collection and analytics.

Key features:
- **REST API**: Simple HTTP endpoints for data submission and querying
- **Thread-safe**: Concurrent data access using shared mutex
- **Time-based filtering**: Query data within specific time ranges
- **Statistical analysis**: Compute mean values from collected data
- **JSON support**: Full JSON request/response handling
- **Cross-platform**: Tested on Linux and Windows
  
# Building
Clone and use CMake directly or via any IDE supporting it. CMake **should**:
+ configure everything automatically,
+ compile and create binaries.

As a result of compilation, binary for server and binary for testing should be created.

## Used tools and libraries for the C++ part
| Tool |  Windows | Ubuntu |
| --- | --- | --- |
| OS version | 10 22H2 | 24.04 |
| GCC | 13.1.0 | 13.2.0 |
| CMake | 3.30.2 | 3.28.3 |
| Git | 2.46.0 | 2.43.0 |
| httplib | 0.14.0 | 0.14.0 |
| nlohmann/json | 3.11.0 | 3.11.0 |
| Catch2 | 3.3.0 | 3.3.0 | 

# Usage
The telemetry server provides a simple HTTP API for collecting and querying telemetry data. The server runs on `localhost:8080` by default and accepts JSON requests.

## Starting the Server
Build and run the server:
```bash
./telemetry-server
```

The server will start and listen on `0.0.0.0:8080`. You'll see a log message confirming the server is running:
```bash
$ <path>/telemetry-server/build/telemetry-server
[INFO]: Starting server on 0.0.0.0:8080
```

## API Endpoints

### POST /paths/:event
Submit telemetry data for a specific event.

**Parameters:**
- `:event` - Event name (path parameter)

**Request Body:**
```json
{
  "date": 1234567890,
  "values": [100, 200, 150, 300]
}
```

**Response:**
- `200 OK` - Data successfully recorded
- `400 Bad Request` - Invalid JSON or missing required fields

### GET /paths/:event/meanLength
Retrieve the mean value for a specific event.

**Parameters:**
- `:event` - Event name (path parameter)
- `resultUnit` - Time unit for the result (`seconds` or `milliseconds`)
- `startTimestamp` (optional) - Start date filter
- `endTimestamp` (optional) - End date filter

**Response:**
```json
{
  "mean": <value>
}
```

## Request Examples

### Submit Telemetry Data
**Request**
```bash
curl -X POST --json '{"date": 1234567890, "values": [100, 200, 150, 300]}' localhost:8080/paths/start
```
**Response:**
```json
{}
```

### Query Mean Value
**Request**
```bash
curl -X GET "localhost:8080/paths/start/meanLength?resultUnit=seconds&startTimestamp=1&endTimestamp=1234567890"
```
**Response:**
```json
{
  "mean": 187
}
```

### Query with Milliseconds
**Request**
```bash
curl -X GET "localhost:8080/paths/start/meanLength?resultUnit=milliseconds"
```
**Response:**
```json
{
  "mean": 187000
}
```

# Configuration
TODO

## Server Settings
TODO

## Logging
TODO

# Architecture
TODO

## Components
TODO

## Data Flow
TODO

# Licensing
Software is released under the MIT license.

The project uses the following open-source software:
| Name | License | Home | Description |
| --- | --- | --- | --- |
| Catch2 | BSL-1.0 | https://github.com/catchorg/Catch2 | testing framework for C++ |
| cpp-httplib | MIT | https://github.com/yhirose/cpp-httplib | HTTP/HTTPS server and client library |
| json | MIT | https://github.com/nlohmann/json| JSON library |

# Testing
For testing of code logic, there is a responsible `Catch2` framework. As it is well integrated with `CMake` it should be straightforward to use. As the first step, build the project. Make sure that the `telemetry-server-tests` target is built. Modern IDEs supporting CMake also support running tests with monitoring of failures. But in case you would like to run it manually, go to the `build\tests` directory, where the⁣ binary `telemetry-server-tests` should be available after building. Calling it directly should produce the following output on Linux:

```
$ ./telemetry-server-tests 
Randomness seeded to: 2788417184
===============================================================================
All tests passed (62 assertions in 3 test cases)
```

As an alternative, CTest can be used to run tests:

```
$ ctest
Test project <path>>/telemetry-server/build/tests
    Start 1: Telemetry
1/3 Test #1: Telemetry ........................   Passed    0.00 sec
    Start 2: PostHandler Tests
2/3 Test #2: PostHandler Tests ................   Passed    0.00 sec
    Start 3: GetHandler Tests
3/3 Test #3: GetHandler Tests .................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 3

Total Test time (real) =   0.02 sec
```