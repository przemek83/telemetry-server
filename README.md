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
   * [Response Format](#response-format)
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
TODO

## API Endpoints
TODO

## Request Examples
TODO

## Response Format
TODO

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
TODO