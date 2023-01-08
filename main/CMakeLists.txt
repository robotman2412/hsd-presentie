# This example code is in the Public Domain (or CC0 licensed, at your option.)
# Unless required by applicable law or agreed to in writing, this
# software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND, either express or implied.

idf_component_register(
    SRCS
        "main.cpp"
        "mqtt.cpp"
        
        "PN532/PN532_HSU/PN532_HSU.cpp"
        "PN532/PN532_I2C/PN532_I2C.cpp"
        "PN532/PN532/emulatetag.cpp"
        "PN532/PN532/snep.cpp"
        "PN532/PN532/mac_link.cpp"
        "PN532/PN532/llcp.cpp"
        "PN532/PN532/PN532.cpp"
        "PN532/PN532_SPI/PN532_SPI.cpp"
    INCLUDE_DIRS
        "." "include" "PN532"
        "PN532/PN532_HSU/"
        "PN532/PN532_I2C/"
        "PN532/PN532/"
        "PN532/PN532_SWHSU/"
        "PN532/PN532_SPI/"
)
