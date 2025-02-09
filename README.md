PPID Spoofing Attack Execution Demonstration

Step 1 – To start the attack demonstration, Opened Process Explorer to verify the original Parent Process ID (PPID) of mspaint.exe before spoofing. The properties showed explorer.exe as its legitimate parent, establishing a baseline for comparison after the spoofing attack.

Step 2 - After obtaining the file path from Visual Studio, Copy the file path. The executable TestPPID.exe was generated in the specified debug folder, ready to be run in the terminal for the spoofing demonstration. 

Step 3 - Then opened the terminal and navigated to the specified file path using the cd command. After that, Used the ls command to list the directory contents and verified that the TestPPID.exe file was successfully generated and ready for execution.

Step 4 - After navigating to the directory, Executed the file by typing .\TestPPID.exe in the terminal. Upon execution, Microsoft Paint (mspaint.exe) launched. In the terminal output, it displayed the current PID of the running program (TestPPID.exe), the PID of notepad.exe (which was spoofed as the parent process), and the PID of mspaint.exe, checking that the PPID spoofing was successfully implemented.

Step 5 - To confirm whether the PPID spoofing attack was successful, Opened Process Explorer and searched for mspaint.exe. In the properties window of mspaint.exe, the Parent process is listed as notepad.exe with PID XXXX. This indicates that the spoofed PPID was successfully assigned to mspaint.exe, even though notepad.exe was not the legitimate parent process that launched it. This manipulation can be used by attackers to disguise malicious processes as benign ones, making them harder to detect in process monitoring tools.











