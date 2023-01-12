# Aura

## TODO

* Document installed Catch2 and version 3 at least (from the devel branch).
* Remove CLion files.

## Benchmarking Signal implementations

### std::tuple based vs std::array based

**tldr**: `std::tuple` with `std::apply()` on `notify()` is faster, takes less memory usage and has less code size.

_Side note:_ `std::tuple` with `std::apply()` built in `Release` mode, optimized the code so much that the call to
`notify()` skipped call to `update()`, and incremented the `num_called` member directly from the call to `notify()`.
Check out the disassembly of the `notify()` for `std::tuple` based solution: there is no `call` instruction inside!

---

The latter needs `virtual` dispatch, thus it will take much more time and space.

Consider disassembly (Ubuntu 20.04 x86\_64), for `std::array` based, for the `notify()` method:

```
0000000000010df0 <_ZN45SignalStdArrayBasedWithCompileTimeConnectionsI5EventJ9Listener1S1_S1_9Listener2S2_S2_9Listener3S3_S3_S3_EE6notifyES0_>:
   10df0:	41 56                	push   %r14
   10df2:	53                   	push   %rbx
   10df3:	50                   	push   %rax
   10df4:	48 89 fb             	mov    %rdi,%rbx
   10df7:	48 8b 7f 08          	mov    0x8(%rdi),%rdi
   10dfb:	48 8b 07             	mov    (%rdi),%rax
   10dfe:	49 89 e6             	mov    %rsp,%r14
   10e01:	4c 89 f6             	mov    %r14,%rsi
   10e04:	ff 10                	call   *(%rax)
   10e06:	48 8b 7b 10          	mov    0x10(%rbx),%rdi
   10e0a:	48 8b 07             	mov    (%rdi),%rax
   10e0d:	4c 89 f6             	mov    %r14,%rsi
   10e10:	ff 10                	call   *(%rax)
   10e12:	48 8b 7b 18          	mov    0x18(%rbx),%rdi
   10e16:	48 8b 07             	mov    (%rdi),%rax
   10e19:	4c 89 f6             	mov    %r14,%rsi
   10e1c:	ff 10                	call   *(%rax)
   10e1e:	48 8b 7b 20          	mov    0x20(%rbx),%rdi
   10e22:	48 8b 07             	mov    (%rdi),%rax
   10e25:	4c 89 f6             	mov    %r14,%rsi
   10e28:	ff 10                	call   *(%rax)
   10e2a:	48 8b 7b 28          	mov    0x28(%rbx),%rdi
   10e2e:	48 8b 07             	mov    (%rdi),%rax
   10e31:	4c 89 f6             	mov    %r14,%rsi
   10e34:	ff 10                	call   *(%rax)
   10e36:	48 8b 7b 30          	mov    0x30(%rbx),%rdi
   10e3a:	48 8b 07             	mov    (%rdi),%rax
   10e3d:	4c 89 f6             	mov    %r14,%rsi
   10e40:	ff 10                	call   *(%rax)
   10e42:	48 8b 7b 38          	mov    0x38(%rbx),%rdi
   10e46:	48 8b 07             	mov    (%rdi),%rax
   10e49:	4c 89 f6             	mov    %r14,%rsi
   10e4c:	ff 10                	call   *(%rax)
   10e4e:	48 8b 7b 40          	mov    0x40(%rbx),%rdi
   10e52:	48 8b 07             	mov    (%rdi),%rax
   10e55:	4c 89 f6             	mov    %r14,%rsi
   10e58:	ff 10                	call   *(%rax)
   10e5a:	48 8b 7b 48          	mov    0x48(%rbx),%rdi
   10e5e:	48 8b 07             	mov    (%rdi),%rax
   10e61:	4c 89 f6             	mov    %r14,%rsi
   10e64:	ff 10                	call   *(%rax)
   10e66:	48 8b 7b 50          	mov    0x50(%rbx),%rdi
   10e6a:	48 8b 07             	mov    (%rdi),%rax
   10e6d:	4c 89 f6             	mov    %r14,%rsi
   10e70:	ff 10                	call   *(%rax)
   10e72:	48 83 c4 08          	add    $0x8,%rsp
   10e76:	5b                   	pop    %rbx
   10e77:	41 5e                	pop    %r14
   10e79:	c3                   	ret    
   10e7a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
```

And the `std::tuple` based Signal:
```
0000000000010cc0 <_ZN45SignalStdTupleBasedWithCompileTimeConnectionsI5EventJ9Listener1S1_S1_9Listener2S2_S2_9Listener3S3_S3_S3_EE6notifyES0_>:
   10cc0:	48 8b 47 50          	mov    0x50(%rdi),%rax
   10cc4:	ff 00                	incl   (%rax)
   10cc6:	48 8b 47 48          	mov    0x48(%rdi),%rax
   10cca:	ff 00                	incl   (%rax)
   10ccc:	48 8b 47 40          	mov    0x40(%rdi),%rax
   10cd0:	ff 00                	incl   (%rax)
   10cd2:	48 8b 47 38          	mov    0x38(%rdi),%rax
   10cd6:	ff 00                	incl   (%rax)
   10cd8:	48 8b 47 30          	mov    0x30(%rdi),%rax
   10cdc:	ff 00                	incl   (%rax)
   10cde:	48 8b 47 28          	mov    0x28(%rdi),%rax
   10ce2:	ff 00                	incl   (%rax)
   10ce4:	48 8b 47 20          	mov    0x20(%rdi),%rax
   10ce8:	ff 00                	incl   (%rax)
   10cea:	48 8b 47 18          	mov    0x18(%rdi),%rax
   10cee:	ff 00                	incl   (%rax)
   10cf0:	48 8b 47 10          	mov    0x10(%rdi),%rax
   10cf4:	ff 00                	incl   (%rax)
   10cf6:	48 8b 47 08          	mov    0x8(%rdi),%rax
   10cfa:	ff 00                	incl   (%rax)
   10cfc:	c3                   	ret    
   10cfd:	0f 1f 00             	nopl   (%rax)
```

`std::tuple` based solution also wins in the contest of code size.

```
$ size tests/host/Release/signal_std_array_based 
   text	   data	    bss	    dec	    hex	filename
1188073	  17160	   7064	1212297	 127f89	tests/host/Release/signal_std_array_based

$ size tests/host/Release/signal_std_tuple_based 
   text	   data	    bss	    dec	    hex	filename
1186869	  16952	   7064	1210885	 127a05	tests/host/Release/signal_std_tuple_based
```
