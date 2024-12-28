.PHONY: build
build:
	./scripts/build.sh

.PHONY: run
run:
	./tmp/powerpilot $(filter-out $@,$(MAKECMDGOALS))

.PHONY: clean
clean:
	rm -f ./tmp/powerpilot
