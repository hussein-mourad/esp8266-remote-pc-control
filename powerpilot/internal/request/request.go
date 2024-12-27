package request

import (
	"fmt"
	"io"
	"net/http"

	"github.com/hussein-mourad/esp8266-remote-pc-control/cli/internal/config"
)

func Send(method, url string) {
	req, err := http.NewRequest(method, url, nil)
	req.SetBasicAuth(config.Username, config.Password)
	res, err := http.DefaultClient.Do(req)
	if err != nil {
		fmt.Println("Error sending request:", err)
		return
	}
	defer res.Body.Close()

	body, err := io.ReadAll(res.Body)
	if err != nil {
		fmt.Println("Error reading response:", err)
		return
	}

	switch res.StatusCode {
	case http.StatusUnauthorized:
		fmt.Println("Error: Unauthorized access")
	case http.StatusOK:
		fmt.Println(string(body))
	case http.StatusBadRequest:
		fmt.Println(string(body))
	default:
		fmt.Println("Error: Unexpected error occured")
		fmt.Println("Status Code: ", res.StatusCode)
		fmt.Println("Make sure esp is running and not in config mode")
	}
}
