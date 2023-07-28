echo "*******TESTING******"
make
count=100
mkdir -p traces
for i in $(seq $count); do
	make run > traces/trace${i}.txt
	cat traces/trace${i}.txt | wc -l
	make run_checker > traces/trace${i}.txt
	tail -n 1 traces/trace${i}.txt
done
echo "********DONE********"
