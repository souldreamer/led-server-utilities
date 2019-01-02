import {
    interpolatePixels,
    interpolateTwoLayers,
    blendPixels,
    blendTwoPixels,
    blendLayers
} from './index';

const white = {r: 1, g: 1, b: 1, n: 1};
const black = {r: 0, g: 0, b: 0, n: 1};
const red = {r: 1, g: 0, b: 0, n: 1};
const green = {r: 0, g: 1, b: 0, n: 1};

console.log(interpolatePixels(white, black, 0.3));
console.log(interpolatePixels(null, black, 0.3));
console.log(interpolatePixels(null, null, 0.3) == null);

console.log(interpolateTwoLayers(null, null, 0.3) == null);
console.log(interpolateTwoLayers([white], [black], 0.3));
console.log(interpolateTwoLayers([white, black], [black, white], 0.3));
console.log(interpolateTwoLayers([white], [black, white], 0.3));
console.log(interpolateTwoLayers(null, [black, white], 0.3));

console.log(blendTwoPixels(red, green, 'Replace'));
console.log(blendTwoPixels(red, green, 'Add'));
console.log(blendTwoPixels(red, green, 'Difference', true));
console.log(blendTwoPixels(white, red, 'Multiply'));

console.log(blendPixels([white, red, green], 'Difference'));
console.log(blendPixels([red, green], 'Add'));

console.log(blendLayers(
    [
        [white, black, red, green],
        [red, red],
        [null, null, green]
    ],
    'Replace'
));
console.log(blendLayers(
    [
        [white, black, red, green],
        [red, red],
        [null, null, green]
    ],
    'Add'
));
console.log(blendLayers(
    [
        [white, black, red, green],
        [red, red],
        [null, null, green]
    ],
    'Dissolve'
));
