ip='224.0.0.15' 

(
echo "write client21/user.join group $ip"; 
sleep 1;
echo "write client31/user.join group $ip"; 
sleep 1;
echo "write client21/user.leave group $ip"; 
sleep 1;
) | telnet localhost 1234
