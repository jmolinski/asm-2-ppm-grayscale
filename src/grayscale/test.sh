set -euo pipefail

test_case() {
    fname="images/$1.ppm"
    output_fname="images/$1.pgm"
    expected_output_fname="images/outs/$1.pgm"
    ./grayscale "$fname"
    if [[ $? -ne 0 ]]; then return 1; fi
    diff "$output_fname" "$expected_output_fname"
    if [[ $? -ne 0 ]]; then echo "$fname test failed"; fi
}

make
rm -f images/*.pgm

for val1 in "apple"  "duck"  "obraz4"  "tea"  "nosacz"; do
  test_case "$val1"
  if [[ $? -ne 0 ]]; then echo "$val1.ppm test failed"; fi
  echo "$val1.ppm test ok"
done

fname="images/nosacz_red.ppm"
output_fname="images/outs/nosacz_red.pgm"
expected_output_fname="images/nosacz_red.pgm"
./grayscale "$fname" 256 0 0
if [[ $? -ne 0 ]]; then echo "$fname.ppm test failed"; fi
diff "$output_fname" "$expected_output_fname"
if [[ $? -ne 0 ]]; then echo "$fname.ppm test failed"; fi
echo "$fname test ok"

fname="images/nosacz_green.ppm"
output_fname="images/outs/nosacz_red.pgm"
expected_output_fname="images/nosacz_red.pgm"
./grayscale "$fname" 0 256 0
if [[ $? -ne 0 ]]; then echo "$fname.ppm test failed"; fi
diff "$output_fname" "$expected_output_fname"
if [[ $? -ne 0 ]]; then echo "$fname.ppm test failed"; fi
echo "$fname test ok"

fname="images/nosacz_blue.ppm"
output_fname="images/outs/nosacz_blue.pgm"
expected_output_fname="images/nosacz_blue.pgm"
./grayscale "$fname" 0 0 256
if [[ $? -ne 0 ]]; then echo "$fname.ppm test failed"; fi
diff "$output_fname" "$expected_output_fname"
if [[ $? -ne 0 ]]; then echo "$fname.ppm test failed"; fi
echo "$fname test ok"
