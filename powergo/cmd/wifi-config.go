/*
Copyright © 2024 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"net/http"

	"github.com/hussein-mourad/esp8266-remote-pc-control/powergo/internal/config"
	"github.com/hussein-mourad/esp8266-remote-pc-control/powergo/internal/request"
	"github.com/spf13/cobra"
)

// configCmd represents the config command
var configCmd = &cobra.Command{
	Use:   "wifi-config",
	Short: "Configure wifi",
	Long: "Enables access point mode to configure wifi\n" +
		"SSID: " + config.APSSID + "\n" +
		"Password: " + config.APPASS + "\n",
	Run: func(cmd *cobra.Command, args []string) {
		request.Send(http.MethodPost, config.BaseURL+"/wifi/config")
	},
}

func init() {
	rootCmd.AddCommand(configCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// configCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// configCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
