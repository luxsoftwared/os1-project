  # extern "C" int test_and_set(&lock, expected, desired)
  # a0 holds address of memory location
  # a1 holds expected value
  # a2 holds desired value
  # a0 holds return value, 0 if successful, !0 otherwise
 .global test_and_set
test_and_set:
  lr.w t0, (a0)         # Load original value.
  bne t0, a1, fail      # Doesn't match, so fail.
  sc.w t0, a2, (a0)     # Try to update.
  bnez t0, test_and_set      # Retry if store-conditional failed.
  li a0, 0              # Set return to success.
  jr ra                 # Return.
fail:
  li a0, 1              # Set return to failure.
  jr ra                 # Return.
