#pragma once

#define MQTT_USER "REPLACE_WITH_DEVICE_USERNAME"
#define MQTT_PASS "REPLACE_WITH_DEVICE_PASSWORD"
#define AP_PASSWORD "REPLACE_WITH_CONFIG_AP_PASSWORD"

// Replace the placeholder body with the ISRG Root X1 PEM certificate body.
#define HIVEMQ_CA_CERT "-----BEGIN CERTIFICATE-----\n" \
                       "REPLACE_WITH_ISRG_ROOT_X1_PEM_BODY\n" \
                       "-----END CERTIFICATE-----\n"
