import QtQuick 2.15
import QtQuick.Shapes 1.15

Item {
    id: radarUI;
    width: 640;
    height: 480;
    property color scaleColor: "#acacac";
    property int fontSize: 16;
    property var model;
    property Component delegate;
    property double radiusOfDetect: 24000;
    property int showAreaZaxis: 0x01 | 0x02 | 0x04 | 0x08;  // 雷达图上只显示设定区域内的目标：空中1、水面2、水下4、岸上8
    readonly property double radius: Math.min(width, height) / 2 - (fontSize * 2);
    readonly property double centerX: width / 2;
    readonly property double centerY: height / 2;

    Canvas {
        id: canvas;
        anchors.fill: parent;
        antialiasing: true;
        onPaint: {
            let ctx = getContext("2d");
            ctx.strokeStyle = scaleColor;
            ctx.lineWidth = 1;
            ctx.beginPath();
            ctx.fillStyle = "#242424";
            ctx.arc(centerX, centerY, radius, 0, 2 * Math.PI);
            ctx.fill();
            ctx.stroke();

            ctx.beginPath();
            ctx.fillStyle = "#1a1a1a";
            ctx.arc(centerX, centerY, 2 / 3 * radius, 0, 2 * Math.PI);
            ctx.fill();
            ctx.stroke();

            ctx.beginPath();
            ctx.fillStyle = "#050505";
            ctx.arc(centerX, centerY, 1 / 3 * radius, 0, 2 * Math.PI);
            ctx.fill();
            ctx.stroke();


            ctx.font = fontSize.toString() + "px Arial";
            ctx.textAlign = "center";
            ctx.textBaseline = "middle";
            ctx.beginPath();
            for(let angle = 0; angle < 360; ++angle) {
                ctx.save();
                ctx.translate(centerX, centerY);
                ctx.rotate(angle * Math.PI / 180);
                let length = 5;
                if(angle % 10 == 0) {
                    length = 15;
                } else if(angle % 5 == 0) {
                    length = 10;
                }
                ctx.moveTo(radius - length, 0);
                ctx.lineTo(radius, 0);

                if(angle % 30 == 0) {
                    ctx.stroke();  //对应下面的beginPath
                    ctx.save(); // 对应下面的restore

                    ctx.lineWidth = 0.3;
                    ctx.fillStyle = scaleColor;
                    ctx.beginPath();  // 对应下面的stroke
                    ctx.moveTo(0, 0);
                    ctx.lineTo(radius, 0);
                    ctx.stroke();

                    ctx.translate(radius + 24, 0);
                    ctx.rotate(-angle * Math.PI / 180);
                    ctx.fillText(((angle + 90) % 360).toString() + "°", 0, 0);


                    ctx.restore();

                    ctx.lineWidth = 1;
                    ctx.strokeStyle = scaleColor;
                    ctx.beginPath();
                }

                if(0 == angle) {
                    ctx.save();
                    ctx.textAlign = "right";
                    ctx.textBaseline = "bottom";
                    ctx.lineWidth = 1;
                    ctx.fillStyle = scaleColor;
                    ctx.fillText(`${Math.round(radiusOfDetect / 1000 * 1 / 3)}km  `, 1 / 3 * radius, 0);
                    ctx.fillText(`${Math.round(radiusOfDetect / 1000 * 2 / 3)}km  `, 2 / 3 * radius, 0);
                    ctx.fillText(`${Math.round(radiusOfDetect / 1000)}km  `, radius, 0);
                    ctx.restore();
                }
                ctx.restore();
            }
            ctx.stroke();
        }
    }

    Shape {
        id: scanner;
        anchors.fill: parent;
        property double radius: radarUI.radius;
        property double centerX: radarUI.centerX;
        property double centerY: radarUI.centerY;

        anchors.centerIn: parent;
        width: Math.min(parent.width, parent.height);
        height: width;
        ShapePath {
            fillGradient: ConicalGradient {
                centerX: scanner.centerX;
                centerY: scanner.centerY;
                angle: 0; // 渐变起始角度（0 表示从右边开始）
                GradientStop { position: 0.0; color: Qt.rgba(00/255,0xFF/255,00/255, 0.35) }
                GradientStop { position: 0.03; color: Qt.rgba(00/255,0xFF/255,00/255, 0.15) }
                GradientStop { position: 0.06; color: Qt.rgba(00/255,0xFF/255,00/255, 0.01) }
                GradientStop { position: 0.09; color: Qt.rgba(00/255,0xFF/255,00/255, 0.0) }
                GradientStop { position: 0.12; color: Qt.rgba(00/255,0xFF/255,00/255, 0.0) }
            }
            strokeColor: "transparent" // 禁用边框
            startX: scanner.centerX;
            startY: scanner.centerY;
            PathLine{x: scanner.centerX + scanner.radius; y: scanner.centerY;}
            PathAngleArc {
                centerX: scanner.centerX;
                centerY: scanner.centerY;
                radiusX: scanner.radius;
                radiusY: scanner.radius;
                startAngle: 0;
                sweepAngle: -30;
            }
            PathLine{x: scanner.centerX; y: scanner.centerY;}
        }

        RotationAnimator on rotation {
            from: 0;
            to: 360;
            duration: 4500;
            loops: Animation.Infinite;
        }
    }

    Connections {
        id: connector;
        target: model;
        property var showIdObjectObj: Object.create(null); // {idNum: showQmlObject, idNum: showQmlObject ...}
        property var roleNameNumObj: model.getRoleNameNumMap(); // {id: 256, angle: 257 ...}
        function onDataChanged() {
            renderTarget(model);
        }

        // 根据数据模型更新或生成目标位置
        function renderTarget(model) {
            let modelList = [];  // [{id: 4001, angle: 121.34 ...}, ...]
            for (let i = 0; i < model.rowCount(); i++) {
                let index = model.index(i, 0);
                let tempModel = {};
                for(let [name, roleNum] of Object.entries(roleNameNumObj)) {
                    tempModel[name] = model.data(index, roleNum);
                };
                if((tempModel.area & radarUI.showAreaZaxis) === tempModel.area) {
                    modelList.push(tempModel);
                }

            }

            for(let [id, show] of Object.entries(showIdObjectObj)) {
                let isIdExist = false;
                for (let modelData of modelList) {
                    if(id == modelData.id) {
                        isIdExist = true;
                        break;
                    }
                }
                if(!isIdExist) {
                    show.destroy(0);
                    delete showIdObjectObj[id];
                }
            }

            let coeff = radiusOfDetect / radius;
            for (let j = 0; j < modelList.length; ++j) {
                let modelData = modelList[j];
                let id = modelData.id;
                let distance = modelData.distance;
                let angle = modelData.angle;
                let newX = centerX + (distance * Math.cos(angle * Math.PI / 180) / coeff);
                let newY = centerY - (distance * Math.sin(angle * Math.PI / 180) / coeff);
                if(id in showIdObjectObj) {
                    showIdObjectObj[id].model = modelData;
                    showIdObjectObj[id].x = newX;
                    showIdObjectObj[id].y = newY;
                } else {
                    let show = delegate.createObject(radarUI, {"model": modelData});
                    show.x = newX;
                    show.y = newY;
                    showIdObjectObj[id] = show;
                }
            }
        }
    }
}

