# YouTube Download Manager

This project provides a graphical interface for downloading YouTube videos. Users can enter a YouTube video link, retrieve video information, and initiate the download process.

## Features
- Fetches and displays video title upon entering a YouTube link.
- Shows downloadable video title.
- Displays download progress (downloaded amount, speed, ETA).
- Shows notification upon successful download.

## Project Structure
```
├── main.cpp                # Main application file
├── mainwindow.ui           # UI file (possibly created using Qt Designer)
├── mainwindow.h            # Header file corresponding to UI file
├── mainwindow.cpp          # Functionality file for UI
├── CMakeLists.txt          # CMake configuration file
├── .gitignore              # Files to be ignored by Git
└── build/                  # Build folder (should be ignored by Git)
```

## Compilation
To compile using C++20 standard:
```bash
g++ -std=c++20 -o YouTubeDownloadManager main.cpp -I /usr/include/qt -lQt5Core -lQt5Widgets -lQt5Network -fPIC
```
Ensure that Qt and Qt Network libraries are installed.

## Dependencies
- Qt (Widgets, Network, Core)
- CMake (if you are using CMake)
- Python backend (Flask) (for running the server API)

## Backend (Flask Server) Setup
The application makes API calls to the following URLs:
- `http://127.0.0.1:5000/get_video_info`
- `http://127.0.0.1:5000/download_video`

Assuming the backend is a Flask application. To set up:
```bash
pip install flask
```
Then, run the server:
```bash
python server.py
```

## Usage
1. Compile the project:
```bash
g++ -std=c++20 -o YouTubeDownloadManager main.cpp -I /usr/include/qt -lQt5Core -lQt5Widgets -lQt5Network -fPIC
```

2. Run the application:
```bash
./YouTubeDownloadManager
```

3. Enter the YouTube link and click `Get Link`.
4. Once the video title is displayed, click `Download` to start downloading.

## Example API Request
To retrieve video information:
```http
POST /get_video_info
{
    "video_link": "https://www.youtube.com/watch?v=C24m5WEYWxE"
}
```

## .gitignore File
```
# Build folder
build/

# CMake user file
CMakeLists.txt.user
*.user.*

# Temporary files created by Qt Creator
*.pro.user*
*.qrc.dep

# Compiled files
*.o
*.obj
*.exe
*.dll
*.so
*.a
*.lib
*.log

# MacOS specific file
.DS_Store
```

## License
This project is created for educational purposes only.

