make
programas_frag=($(ls samples | grep -oP '.*\.frag' | grep -oP '.*\.'))
programas_decaf=($(ls samples | grep -oP '.*\.decaf' | grep -oP '.*\.'))

for item in ${programas_frag[@]}; do
  (./dcc < "samples/"$item"frag") > "samples/"$item"out1" 2>&1
done

for item in ${programas_decaf[@]}; do
  (./dcc < "samples/"$item"decaf") > "samples/"$item"out1" 2>&1
done

programas=($(ls samples | grep -oP '.*\.' | uniq))

for item in ${programas[@]}; do
  echo Archivo: $item
  diff --text "samples/"$item"out" "samples/"$item"out1"
done


rm samples/*.out1
make clean
