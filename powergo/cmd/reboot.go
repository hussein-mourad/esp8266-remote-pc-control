/*
Copyright © 2024 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"net/http"

	"github.com/spf13/cobra"
)

var rebootEsp = false

// rebootCmd represents the reboot command
var rebootCmd = &cobra.Command{
	Use:   "reboot",
	Short: "Reboot handler",
	Run: func(cmd *cobra.Command, args []string) {
		if rebootEsp {
			req.Send(http.MethodPost, cfg.API.URL+"/esp/reboot")
		} else {
			req.Send(http.MethodPost, cfg.API.URL+"/power/reboot")
		}
	},
}

func init() {
	rootCmd.AddCommand(rebootCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// rebootCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	rebootCmd.Flags().BoolVar(&rebootEsp, "esp", rebootEsp, "Reboot the esp itself")
}
