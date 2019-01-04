declare module 'led-server-utilities' {
	interface Color {
		n: number;
		r: number;
		g: number;
		b: number;
	}
	
	type ColorArray = (Color | null)[];
	export type BlendModeType =
		'Add'
		| 'Subtract'
		| 'Multiply'
		| 'Screen'
		| 'Overlay'
		| 'HardLight'
		| 'SoftLight'
		| 'Divide'
		| 'Difference'
		| 'Darken'
		| 'Lighten'
		| 'Replace'
		| 'Dissolve';
	export type InterpolationType =
		'linear'
		| 'Linear'
		| 'easeIn'
		| 'EaseIn'
		| 'easeOut'
		| 'EaseOut'
		| 'easeInOut'
		| 'EaseInOut';
	
	export function interpolatePixels(color1: null, color2: null, x: number, interpolationName?: InterpolationType, skipIntensity?: boolean): null;
	export function interpolatePixels(color1: Color | null, color2: Color | null, x: number, interpolationName?: InterpolationType, skipIntensity?: boolean): Color;

	export function interpolateTwoLayers(layer1: null, layer2: null, x: number, interpolationName?: InterpolationType, skipIntensity?: boolean): ColorArray;
	export function interpolateTwoLayers(layer1: ColorArray | null, layer2: ColorArray | null, x: number, interpolationName?: InterpolationType, skipIntensity?: boolean): ColorArray;
	
	export function blendPixels(pixels: ColorArray, blendMode?: BlendModeType, applyToIntensity?: boolean): Color | null;

	export function blendTwoPixels(color1: null, color2: null, blendMode?: BlendModeType, applyToIntensity?: boolean): null;
	export function blendTwoPixels(color1: Color | null, color2: Color | null, blendMode?: BlendModeType, applyToIntensity?: boolean): Color;

	export function blendLayers(layers: (ColorArray | null)[], blendMode?: BlendModeType, applyToIntensity?: boolean): ColorArray | null;

	export function applyKernelToLayer(layer: ColorArray, kernel: number[], rotate?: boolean, skipIntensity?: boolean, paddingColor?: Color | null): ColorArray;
	export function applyKernelToLayer(layer: null, kernel: null | number[], rotate?: boolean, skipIntensity?: boolean, paddingColor?: Color | null): null;
	export function applyKernelToLayer(layer: null | ColorArray, kernel: null, rotate?: boolean, skipIntensity?: boolean, paddingColor?: Color | null): null;

	export function dimLayer(layer: ColorArray, dimMultiplier: number, skipIntensity?: boolean): ColorArray;
	export function dimLayer(layer: null, dimMultiplier: number, skipIntensity?: boolean): null;
}
