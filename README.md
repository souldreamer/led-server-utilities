# Led Server Utilities

Moved some utility function (interpolations & blendings)
from JavaScript to a node native add-on (which will
_hopefully_ speed up some of the calculations of the LED
server that happen pretty often).

## Functions
* `interpolatePixels` - interpolates between two colors
* `interpolateTwoLayers` - interpolates two layers pixel by pixel
* `blendPixels` - blends an array of pixels
* `blendTwoPixels` - blends two colors
* `blendLayers` - blends an array of layers pixel by pixel

## Interpolations
* `Linear`
* `EaseIn`
* `EaseOut`
* `EaseInOut`

## Blend Modes
`Add`, `Subtract`, `Multiply`, `Screen`, `Overlay`,
`HardLight`, `SoftLight`, `Divide`, `Difference`,
`Darken`, `Lighten`, `Replace`, `Dissolve`

## Examples
```javascript
const white = {r: 1, g: 1, b: 1, n: 1};
const black = {r: 0, g: 0, b: 0, n: 1};
const red = {r: 1, g: 0, b: 0, n: 1};
const green = {r: 0, g: 1, b: 0, n: 1};

interpolatePixels(white, black, 0.3);
interpolatePixels(null, black, 0.3);
interpolatePixels(null, null, 0.3);

interpolateTwoLayers(null, null, 0.3);
interpolateTwoLayers([white], [black], 0.3);
interpolateTwoLayers([white, black], [black, white], 0.3);
interpolateTwoLayers([white], [black, white], 0.3);
interpolateTwoLayers(null, [black, white], 0.3);

blendTwoPixels(red, green, 'Replace');
blendTwoPixels(red, green, 'Add');
blendTwoPixels(red, green, 'Difference', true);
blendTwoPixels(white, red, 'Multiply');

blendPixels([white, red, green], 'Difference');
blendPixels([red, green], 'Add');

blendLayers(
    [
        [white, black, red, green],
        [red, red],
        [null, null, green]
    ],
    'Replace'
);
blendLayers(
    [
        [white, black, red, green],
        [red, red],
        [null, null, green]
    ],
    'Add'
);
blendLayers(
    [
        [white, black, red, green],
        [red, red],
        [null, null, green]
    ],
    'Dissolve'
);
```