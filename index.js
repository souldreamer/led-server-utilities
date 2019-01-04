const {
    interpolatePixels,
    interpolateTwoLayers,
    blendPixels,
    blendTwoPixels,
    blendLayers,
    applyKernelToLayer,
    dimLayer
} = require('./build/Release/LedServerUtilities.node');

module.exports = {
    interpolatePixels,
    interpolateTwoLayers,
    blendPixels,
    blendTwoPixels,
    blendLayers,
    applyKernelToLayer,
    dimLayer
};
