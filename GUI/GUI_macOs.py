import tkinter as tk
import serial
import re
import matplotlib.pyplot as plt
import serial.tools.list_ports
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

# Globale Variablen
ser = None
messung_aktiv = 0
time_values = []
position_values = []
counter = 0

def find_arduino_port():
    ports = list(serial.tools.list_ports.comports())
    for port in ports:
        if "usbmodem" in port.device or "usbserial" in port.device:
            return port.device
    raise IOError("Kein Arduino-Port gefunden.")

def ensure_serial_connection():
    global ser
    if ser is None:
        try:
            port = find_arduino_port()
            ser = serial.Serial(port, 9600)
            print(f"✅ Verbunden mit {port}")
        except Exception as e:
            print(f"⚠️ Fehler bei Verbindung: {e}")
            return False
    return True

def toggle_measurement(*args):
    global counter, messung_aktiv
    if not ensure_serial_connection():
        abstand_var.set(False)
        return

    if abstand_var.get():
        ser.write("M\n".encode())
        messung_aktiv = 1
    else:
        ser.write("MN\n".encode())
        messung_aktiv = 0
        ser.reset_input_buffer()

def toggle_ref(*args):
    if not ensure_serial_connection():
        ref_var.set(False)
        return

    if ref_var.get():
        ser.write("R\n".encode())
    else:
        print("⚠️ Keine Referenzfahrt gestartet")

def send_text():
    if not ensure_serial_connection():
        return

    text = text_entry.get().strip()
    if text:
        ser.write(f"Beschriftung:{text}\n".encode())
    else:
        print("⚠️ Kein Text gesendet")

def start_action():
    if not ensure_serial_connection():
        return
    ser.write("START\n".encode())

def stop_action():
    if not ensure_serial_connection():
        return
    ser.write("STOP\n".encode())

def receive_data():
    global counter
    if ser and ser.in_waiting > 0:
        received_message = ser.readline().decode("latin-1").strip()
        try:
            if "mm" in received_message and messung_aktiv:
                distance = int(re.search(r'\d+', received_message).group())
                time_values.append(counter)
                position_values.append(distance)
                counter += 1
                update_plot()
            else:
                print(f"{received_message}")
        except ValueError:
            print(f"⚠️ Fehlerhafte Nachricht: {received_message}")
    
    root.after(100, receive_data)

def update_plot():
    ax.clear()
    ax.plot(time_values, position_values, marker="x", linestyle="-", color="red")
    ax.set_title("Abstandsmessung über Zeit")
    ax.set_xlabel("Zeit")
    ax.set_ylabel("Position")
    canvas.draw()

# --- GUI Aufbau ---
root = tk.Tk()
root.title("Arduino Steuerung")

# Frames
top_frame = tk.Frame(root)
top_frame.pack(side=tk.TOP, pady=10)

plot_frame = tk.Frame(root)
plot_frame.pack(side=tk.TOP, pady=10)

bottom_frame = tk.Frame(root)
bottom_frame.pack(side=tk.TOP, pady=10)

# Checkboxen
ref_var = tk.BooleanVar()
ref_checkbox = tk.Checkbutton(top_frame, text="Referenzfahrt", variable=ref_var)
ref_checkbox.pack(side=tk.LEFT, padx=5)
ref_var.trace_add("write", toggle_ref)

abstand_var = tk.BooleanVar()
abstand_checkbox = tk.Checkbutton(top_frame, text="Abstand messen", variable=abstand_var)
abstand_checkbox.pack(side=tk.LEFT, padx=5)
abstand_var.trace_add("write", toggle_measurement)

# Plot kleiner darstellen
fig, ax = plt.subplots(figsize=(6, 3))
canvas = FigureCanvasTkAgg(fig, master=plot_frame)
canvas.get_tk_widget().pack()

# Textfeld & Buttons
text_entry = tk.Entry(bottom_frame, width=30)
text_entry.pack(pady=5)

send_button = tk.Button(bottom_frame, text="Sende Text", command=send_text)
send_button.pack(pady=5)

start_button = tk.Button(bottom_frame, text="Start", command=start_action)
start_button.pack(pady=5)

stop_button = tk.Button(bottom_frame, text="Not-Aus", command=stop_action, bg="red", fg="white")
stop_button.pack(pady=5)

# Datenempfang starten
root.after(100, receive_data)
root.mainloop()