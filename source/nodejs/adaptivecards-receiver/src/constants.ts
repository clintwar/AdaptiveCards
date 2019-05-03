// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
// TOOD: Can I pull this from the samples folder rather than copying it here?

export const defaultConfigPayload: string = `{
	"supportsInteractivity": true,
	"strongSeparation": {
		"spacing": 40,
		"lineThickness": 1,
		"lineColor": "#EEEEEE"
	},
	"fontFamily": "Segoe UI",
	"fontSizes": {
		"small": 12,
		"normal": 14,
		"medium": 17,
		"large": 21,
		"extraLarge": 26
	},
	"fontWeights": {
		"lighter": 200,
		"normal": 400,
		"bolder": 600
	},
	"colors": {
		"dark": {
			"normal": "#333333",
			"subtle": "#EE333333"
		},
		"light": {
			"normal": "#FFFFFF",
			"subtle": "#88FFFFFF"
		},
		"accent": {
			"normal": "#2E89FC",
			"subtle": "#882E89FC"
		},
		"attention": {
			"normal": "#FFD800",
			"subtle": "#DDFFD800"
		},
		"good": {
			"normal": "#00FF00",
			"subtle": "#DD00FF00"
		},
		"warning": {
			"normal": "#FF0000",
			"subtle": "#DDFF0000"
		}
	},
	"imageSizes": {
		"small": 40,
		"medium": 80,
		"large": 160
	},
	"actions": {
		"maxActions": 5,
		"separation": {
			"spacing": 20
		},
		"buttonSpacing": 20,
		"stretch": false,
		"showCard": {
			"actionMode": "inline",
			"inlineTopMargin": 16,
			"backgroundColor": "#08000000",
			"padding": {
				"top": 16,
				"right": 16,
				"bottom": 16,
				"left": 16
			}
		},
		"actionsOrientation": "horizontal",
		"actionAlignment": "left"
	},
	"adaptiveCard": {
		"backgroundColor": "#00000000",
		"padding": {
			"left": 20,
			"top": 20,
			"right": 20,
			"bottom": 20
		}
	},
	"container": {
		"separation": {
			"spacing": 20
		},
		"normal": {},
		"emphasis": {
			"backgroundColor": "#EEEEEE",
			"borderColor": "#AAAAAA",
			"borderThickness": {
				"top": 1,
				"right": 1,
				"bottom": 1,
				"left": 1
			},
			"padding": {
				"top": 10,
				"right": 10,
				"bottom": 10,
				"left": 10
			}
		}
	},
	"textBlock": {
		"color": "dark",
		"separations": {
			"small": {
				"spacing": 20
			},
			"normal": {
				"spacing": 20
			},
			"medium": {
				"spacing": 20
			},
			"large": {
				"spacing": 20
			},
			"extraLarge": {
				"spacing": 20
			}
		}
	},
	"image": {
		"size": "medium",
		"separation": {
			"spacing": 20
		}
	},
	"imageSet": {
		"imageSize": "medium",
		"separation": {
			"spacing": 20
		}
	},
	"factSet": {
		"separation": {
			"spacing": 20
		},
		"title": {
			"color": "dark",
			"size": "normal",
			"isSubtle": false,
			"weight": "bolder",
			"wrap": true,
			"maxWidth": 150
		},
		"value": {
			"color": "dark",
			"size": "normal",
			"isSubtle": false,
			"weight": "normal",
			"wrap": true
		},
		"spacing": 10
	},
	"input": {
		"separation": {
			"spacing": 20
		}
	},
	"columnSet": {
		"separation": {
			"spacing": 20
		}
	},
	"column": {
		"separation": {
			"spacing": 20
		}
	}
}`;

export const defaultPayload: string = `{
	"$schema": "http://adaptivecards.io/schemas/adaptive-card.json",
	"type": "AdaptiveCard",
	"version": "1.0",
	"body": [
		{
			"type": "Container",
			"items": [
				{
					"type": "TextBlock",
					"text": "Connecting...",
					"weight": "bolder",
					"size": "medium"
				}
			]
		}
	]
}`;