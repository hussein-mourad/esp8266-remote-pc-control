BUILD_PATH := ./tmp/powergo
INSTALL_PATH := $(shell go env GOPATH)/bin


.PHONY: build
build:
	go mod tidy
	go build -o $(BUILD_PATH)

.PHONY: install
install: build
	cp $(BUILD_PATH) $(INSTALL_PATH)

.PHONY: run
run: build
	./$(BUILD_PATH) $(filter-out $@,$(MAKECMDGOALS))

.PHONY: clean
clean:
	rm -rf ./tmp
