pkg:
	(cd .. && zip -r lightspeed.zip lightspeed -x \*~ -x \*.o -x \*.obj -x \*.dll -x \*.exe -x \*.lib -x \*.exp -x \*.map -x \*.mex\* -x \*.manifest -x \*CVS\* -x \*xrepmat.m -x \*Makefile)

clean:
	rm -f *~ */*~ *.dll *.exe *.obj *.o *.mex* */*.mex* *.lib *.exp *.map
