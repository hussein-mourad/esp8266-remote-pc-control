/*
Copyright © 2024 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"net/http"

	"github.com/hussein-mourad/esp8266-remote-pc-control/cli/internal/config"
	"github.com/hussein-mourad/esp8266-remote-pc-control/cli/internal/request"
	"github.com/spf13/cobra"
)

var force = false

// downCmd represents the down command
var downCmd = &cobra.Command{
	Use:   "down",
	Short: "Turns off the pc",
	Run: func(cmd *cobra.Command, args []string) {
		endpoint := "off"
		if force {
			endpoint = "forceoff"
		}
		request.Send(http.MethodPost, config.BaseURL+"/power/"+endpoint)
	},
}

func init() {
	rootCmd.AddCommand(downCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// downCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	downCmd.Flags().BoolVarP(&force, "force", "f", force, "Force off")
}
