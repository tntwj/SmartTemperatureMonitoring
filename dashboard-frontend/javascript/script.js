const SERVER_HOST = "https://hamster-holy-mutt.ngrok-free.app";
const SERVER_PORT = 8080;

const TEMPERATURE_PATH = "/api/samples";
const REPORTS_PATH = "/api/reports";
const CONFIG_PATH = "/api/config";
const SWITCH_MODE_PATH = "/api/request_mode_switch";
const SWITCH_ALARM_PATH = "/api/request_alarm_switch";

let isManualMode = false;

async function fetchTemperatureData() {
    const response = await fetch(`${SERVER_HOST}${TEMPERATURE_PATH}`);
    const data = await response.json();
    console.log("Temperature Data:", data);
}

async function fetchConfigData() {
    const response = await fetch(`${SERVER_HOST}${CONFIG_PATH}`);
    const data = await response.json();
    console.log("Config Data:", data);

    // Update UI based on system state
    const systemState = document.getElementById('systemState');
    const windowLevel = document.getElementById('windowLevel');

    systemState.textContent = data.status;
    windowLevel.textContent = data.windowLevel;

    isManualMode = data.mode === "MANUAL";

    // Disable the range input if not in manual mode
    const windowControl = document.getElementById('windowControl');
    windowControl.disabled = !isManualMode;
}

document.getElementById('manualModeToggle').addEventListener('click', async () => {
    const response = await fetch(`${SERVER_HOST}${SWITCH_MODE_PATH}`, { method: 'POST' });
    const result = await response.json();
    console.log("Manual Mode Toggle Result:", result);

    // Refresh the config data to update the UI
    fetchConfigData();
});

document.getElementById('resolveAlarm').addEventListener('click', async () => {
    const response = await fetch(`${SERVER_HOST}${SWITCH_ALARM_PATH}`, { method: 'POST' });
    const result = await response.json();
    console.log("Alarm Resolved:", result);

    // Refresh the config data to update the UI
    fetchConfigData();
});

document.getElementById('windowControl').addEventListener('input', (event) => {
    if (!isManualMode) {
        alert("Cannot change window level in automatic mode.");
        event.target.value = 0; // Reset the range input value
    }
});

// Initial data fetch
fetchTemperatureData();
fetchConfigData();
