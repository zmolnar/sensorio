# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

idf_component_register(SRCS "main.c"
                            "MicroNMEA.cpp"
                            "Power.cpp"
                            "core/BatteryMonitorThread.cpp"
                            "core/BeepControlThread.cpp"
                            "core/GpsManagerThread.cpp"
                            "core/LvglThread.cpp"
                            "core/ImuManagerThread.cpp"
                            "core/PressureReaderThread.cpp"
                            "core/DataFilterThread.cpp"
                            "core/DataLoggerThread.cpp"
                            "kalmanfilter/Matrix.cpp"
                            "kalmanfilter/MerweScaledSigmaPoints.cpp"
                            "kalmanfilter/SigmaPoints.cpp"
                            "kalmanfilter/Ukf.cpp"
                            "dashboard/Dashboard.cpp"
                            "dashboard/LocalTime.cpp"
                            "drivers/bno055/bst/bst_bno055.c"
                            "drivers/bno055/bno055.cpp"
                            "drivers/encoder/Encoder.cpp"
                            "drivers/lcd/SharpLcd.cpp"
                            "drivers/ms5611/ms5611.cpp"
                            "gui/Sensorio.c"
                            "gui/images/paraglider.c"
                            "gui/screens/BpsData.c"
                            "gui/screens/GpsData.c"
                            "gui/screens/ImuData.c"
                            "gui/screens/Startup.c"
                            "gui/screens/SystemStatus.c"
                            "gui/screens/Variometer.c"
                            "gui/screens/VariometerSettings.c"
                            )
