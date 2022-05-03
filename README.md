# Introduction
Implementation of SM4 and a variety of its optimized version.
The following optimization strategy is used:

- Loop unrolling
- Multithread
- Look up table
- SIMD(AVX ISA)


# Optimization strategy
## Algorithm Level
Implement T transform by Look up Table Algorithm.

## Thread Level
For a large number of encryption tasks, multi-threaded acceleration is used

## Compiler Level
- MSVC X64 Release
- Use the "gestalt" of loop unrolling to reduce instructions

## Data Level
Combine **SIMD(AVX)** with look up table algorithm. In fact, this way implements parallel table lookup. 



# Test
<style type="text/css">
.tg  {border-collapse:collapse;border-spacing:0;}
.tg td{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  overflow:hidden;padding:10px 5px;word-break:normal;}
.tg th{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg .tg-c3ow{border-color:inherit;text-align:center;vertical-align:top}
.tg .tg-7btt{border-color:inherit;font-weight:bold;text-align:center;vertical-align:top}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-7btt">strategy\scale</th>
    <th class="tg-7btt">1000000</th>
    <th class="tg-7btt">10000000</th>
    <th class="tg-7btt">100000000</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-7btt">origin</td>
    <td class="tg-c3ow">98MB/s</td>
    <td class="tg-c3ow">101MB/s</td>
    <td class="tg-c3ow">102MB/s</td>
  </tr>
  <tr>
    <td class="tg-7btt">+ Loop unrolling</td>
    <td class="tg-c3ow">103MB/s</td>
    <td class="tg-c3ow">104MB/s</td>
    <td class="tg-c3ow">103MB/s</td>
  </tr>
  <tr>
    <td class="tg-7btt">+ Look-up table</td>
    <td class="tg-c3ow">148MB/s</td>
    <td class="tg-c3ow">161MB/s</td>
    <td class="tg-c3ow">163MB/s</td>
  </tr>
  <tr>
    <td class="tg-7btt">+ Look-up table + SIMD</td>
    <td class="tg-c3ow">396MB/s</td>
    <td class="tg-c3ow">405MB/s</td>
    <td class="tg-c3ow">404MB/s</td>
  </tr>
</tbody>
</table>
