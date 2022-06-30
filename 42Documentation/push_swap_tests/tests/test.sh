echo "*******TESTING******"
make
count=100
mkdir traces
for i in $(seq $count); do
	make run > tests/traces/trace${i}.txt
	cat tests/traces/trace${i}.txt | wc -l
	make run_checker > tests/traces/trace${i}.txt
	tail -n 1 tests/traces/trace${i}.txt
done
echo "********DONE********"
