let cvip = require('../lib');
console.log(cvip.imageCut('test.jpg', 'tmp.jpg', cvip.jpeg, 1000, 200, 100, 120));
console.log(cvip.imageMatch('tmp.jpg', 'test.jpg'));
console.log(cvip.imageSave('tmp.jpg', 'tmp.png', cvip.png));
