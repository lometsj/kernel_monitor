build:
	make -C src build
	make -C trace all
	make -C trace_user build
	mkdir -p release/mod/
	mv trace/*.ko release/mod/
	cp bin/* release/
	cp config/* release/
	cd release;rm -rf v1.0.0.zip;zip -r v1.0.0.zip ./*

clean:
	make -C src clean
	make -C trace clean
	make -C trace_user clean