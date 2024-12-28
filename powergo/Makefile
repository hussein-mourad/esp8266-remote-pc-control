.PHONY: build
build:
	./scripts/build.sh

.PHONY: run
run: build
	./tmp/powergo $(filter-out $@,$(MAKECMDGOALS))

.PHONY: clean
clean:
	rm -f ./tmp/powergo
