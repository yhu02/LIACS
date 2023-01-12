interface ICalcArrow {
    fromRect: DOMRect,
    toRect: DOMRect,
    gridRect: DOMRect,
    offset: number
}

interface RCalcArrow {
    from: {x: number, y: number};
    to: {x: number, y: number};
    length: number;
    arrowDirection: number
}

interface RCalcAnchor {
    from: {x: number, y: number};
    to: {x: number, y: number};
    fromOrientation?: number;
    toOrientation?: number;
}

interface RCalcBox {
    top: number,
    left: number,
    right: number,
    bottom: number
}

interface RAnchorPos {
    x: number,
    y: number
}

const calcBox: (box: DOMRect, canvas: DOMRect) => RCalcBox = (
    box,
    canvas
) => {
    return {
        top: box.top - canvas.top,
        bottom: box.top + box.height - canvas.top,
        left: box.left - canvas.left,
        right: box.left + box.width - canvas.left
    }
}

const calcAnchorPos: (
    box: DOMRect,
    canvas: DOMRect,
    anchor: number,
    offset: number
) => RAnchorPos = (
    box,
    canvas,
    anchor,
    offset
) => {
    if (anchor === 0) {
        // Top
        return {
            x: box.left - canvas.x + (box.width / 2) + 16 * offset,
            y: box.top - canvas.y
        }
    } else if (anchor === 1) {
        // Left
        return {
            x: box.left - canvas.x,
            y: box.bottom - canvas.y - (box.height / 2) + 16 * offset
        }
    } else if (anchor === 2) {
        // Bottom
        return {
            x: box.left - canvas.x + (box.width / 2) + 16 * offset,
            y: box.bottom - canvas.y
        }
    } else {
        // Right
        return {
            x: box.right - canvas.x,
            y: box.top - canvas.y + (box.height / 2) + 16 * offset
        }
    }
}

const calcAnchor: (
    fromRect: DOMRect, 
    toRect: DOMRect, 
    canvas: DOMRect,
    offset: number
) => RCalcAnchor = (
    fromRect, 
    toRect,
    canvas,
    offset
) => {
    const fromBox = calcBox(fromRect, canvas)
    const toBox = calcBox(toRect, canvas)

    let anchorDistances = [
        toBox.top - fromBox.bottom,
        toBox.left - fromBox.right,
        fromBox.top - toBox.bottom,
        fromBox.left - toBox.right
    ]
    
    let anchor = anchorDistances.indexOf(
        Math.max.apply(Math, anchorDistances)
    )

    let fromAnchor = calcAnchorPos(
        fromRect, 
        canvas, 
        ((anchor + 2) % 4),
        offset
    )
    
    let toAnchor = calcAnchorPos(
        toRect, 
        canvas, 
        anchor,
        offset
    )

    return {
        from: fromAnchor,
        to: toAnchor,
        fromOrientation: ((anchor + 2) % 4),
        toOrientation: anchor
    }
}

const calcDirection: (
    anchor: RCalcAnchor
) => number = (
    anchor
) => {
    return (Math.atan2(
            anchor.from.y - anchor.to.y, 
            anchor.from.x - anchor.to.x
        ) * 180 / Math.PI - 180) % 360;
}

const calcLength: (
    anchor: RCalcAnchor
) => number = (
    anchor
) => {
    return Math.hypot(
        anchor.to.x - anchor.from.x,
        anchor.to.y - anchor.from.y
    )
}

export const calcArrow: (value: ICalcArrow) => RCalcArrow = ({
    fromRect: from,
    toRect: to,
    gridRect: canvas,
    offset
}) => {
    let anchor = calcAnchor(
        from, 
        to, 
        canvas, 
        offset
    )

    let direction = calcDirection(anchor)
    let length = calcLength(anchor)
    
    return {
        ...anchor,
        length: length,
        arrowDirection: direction
    }
};