document.addEventListener('DOMContentLoaded',function(){


   
   let view={
    xmin: -2.0,
    xmax:1.0,
    ymin:-1.5,
    ymax:1.5,
    zoomFactor:0.8,
    red: 0.88,    
    green: 0.37,   
    blue: 0.26
   };


   const img=document.getElementById('fractal-img');
   const xminInput=document.getElementById('xmin');
   const xmaxInput=document.getElementById('xmax');
   const yminInput=document.getElementById('ymin');
   const ymaxInput=document.getElementById('ymax');
   const generatebtn=document.getElementById('generate');
   const zoominbtn=document.getElementById('zoomin');
   const zoomoutbtn=document.getElementById('zoomout');
   const redSlider=document.getElementById('red');
   const greenSlider=document.getElementById('green');
   const blueSlider=document.getElementById('blue');
  


    function updateImage()
    {
      const params = new URLSearchParams({
            xmin: view.xmin,
            xmax: view.xmax,
            ymin: view.ymin,
            ymax: view.ymax,
            red: view.red,
            green: view.green,
            blue: view.blue
        });
        const url = `/mandelbrot?${params.toString()}`;
    console.log('Fractal request →', url);

        img.src=`/mandelbrot?${params.toString()}`;
        xminInput.value = view.xmin.toFixed(2);
        xmaxInput.value = view.xmax.toFixed(2)
        yminInput.value = view.ymin.toFixed(2);
        ymaxInput.value = view.ymax.toFixed(2);
        redSlider.value=view.red.toFixed(2);
        greenSlider.value=view.green.toFixed(2);
        blueSlider.value=view.blue.toFixed(2);
        
    }
   
   generatebtn.addEventListener('click',function(e){
    e.preventDefault();
     console.log("Generate clicked"); 
     view.xmin=parseFloat(xminInput.value);
     view.xmax=parseFloat(xmaxInput.value);
     view.ymin=parseFloat(yminInput.value);
     view.ymax=parseFloat(ymaxInput.value);
     view.red=parseFloat(redSlider.value);
     view.green=parseFloat(greenSlider.value);
     view.blue=parseFloat(blueSlider.value);
     updateImage();

   });

   zoominbtn.addEventListener('click',function(){
    const xcent=(view.xmin+view.xmax)/2;
    const ycent=(view.ymin+view.ymax)/2;
    const xRange=(view.xmax-view.xmin)*view.zoomFactor;
    const yRange=(view.ymax-view.ymin)*view.zoomFactor;

    view.xmin=xcent-xRange/2;
    view.ymin=ycent-yRange/2;
    view.xmax=xcent+xRange/2;
    view.ymax=ycent+yRange/2;
    

    updateImage();
   });

    zoomoutbtn.addEventListener('click',function(){
    const xcent=(view.xmin+view.xmax)/2;
    const ycent=(view.ymin+view.ymax)/2;
    const xRange=(view.xmax-view.xmin)/view.zoomFactor;
    const yRange=(view.ymax-view.ymin)/view.zoomFactor;

    view.xmin=xcent-xRange/2;
    view.ymin=ycent-yRange/2;
    view.xmax=xcent+xRange/2;
    view.ymax=ycent+yRange/2;

    updateImage();
   });
   
  updateImage();

  console.log('Color values:', {
    red: redSlider.value,
    green: greenSlider.value,
    blue: blueSlider.value
});
});