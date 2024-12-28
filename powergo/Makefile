.PHONY: build
build:
	go mod tidy
	./scripts/build.sh

.PHONY: install
install: build
	cp ./tmp/powergo $(shell go env GOPATH)/bin

.PHONY: run
run: build
	./tmp/powergo $(filter-out $@,$(MAKECMDGOALS))

.PHONY: clean
clean:
	rm -f ./tmp/powergo
